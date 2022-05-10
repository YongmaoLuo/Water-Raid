/* * Video and Audio driver for the Water Raid Project
 *
 * A Platform device implemented using the misc subsystem
 *
 * Yongmao Luo
 * Columbia University
 *
 * References:
 * Linux source: Documentation/driver-model/platform.txt
 *               drivers/misc/arm-charlcd.c
 * http://www.linuxforu.com/tag/linux-device-drivers/
 * http://free-electrons.com/docs/
 *
 * "make" to build
 * insmod water_video.ko
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "water_video.h"

#define DRIVER_NAME "water_video"

/* Device registers */
#define BOUNDARY0(x) (x)
#define BOUNDARY1(x) ((x)+2)
#define BOUNDARY2(x) ((x)+4)
#define BOUNDARY3(x) ((x)+6)
#define SHIFT(x) ((x)+8)
#define SCOREBOARDX(x) ((x) + 64)
#define SCOREBOARDY(x) ((x) + 66)
#define DIGIT1X(x) ((x) + 68)
#define DIGIT1Y(x) ((x) + 70)
#define DIGIT1IMG(x) ((x) + 72)
#define DIGIT2X(x) ((x) + 74)
#define DIGIT2Y(x) ((x) + 76)
#define DIGIT2IMG(x) ((x) + 78)
#define DIGIT3X(x) ((x) + 80)
#define DIGIT3Y(x) ((x) + 82)
#define DIGIT3IMG(x) ((x) + 84)
#define FUELGAUGEX(x) ((x) + 86)
#define FUELGAUGEY(x) ((x) + 88)
#define INDICATORX(x) ((x) + 90)
#define INDICATORY(x) ((x) + 92)
#define SHOOTAUDIO(x) ((x)+94)
#define HITAUDIO(x) ((x)+96)
#define EXPLODEAUDIO(x) ((x)+98)

#define FUELGAUGEHALFLENGTH 40

/*
 * Information about our device
 */
struct water_video_dev {
	struct resource res; /* Resource: our registers */
	void __iomem *virtbase; /* Where registers can be accessed in memory */
  	water_video_arg_boundary argBoundary;
      water_video_arg_position argPosition;
      water_video_arg_fuel argFuel;
      water_video_arg_score argScore;
      water_video_arg_init argInit;
      water_audio_arg argAudio;
} dev;

/*
 * Write segments of a single digit
 * Assumes digit is in range and the device information has been set up
 */
// static void write_background(water_video_color_t *background)
// {
// 	iowrite8(background->red, BG_RED(dev.virtbase) );
// 	iowrite8(background->green, BG_GREEN(dev.virtbase) );
// 	iowrite8(background->blue, BG_BLUE(dev.virtbase) );
// 	dev.background = *background;
// }

static void writeBoundary(water_video_arg_boundary *arg)
{
    iowrite16(arg->boundary.river1_left, BOUNDARY0(dev.virtbase) );
    iowrite16(arg->boundary.river1_right, BOUNDARY1(dev.virtbase) );
    iowrite16(arg->boundary.river2_left, BOUNDARY2(dev.virtbase) );
    iowrite16(arg->boundary.river2_right, BOUNDARY3(dev.virtbase) );
    iowrite16(arg->shift, SHIFT(dev.virtbase));
    dev.argBoundary = *arg;
}

/*
 * Write 16 bits for each location variable
 * Assumes digit is in range and the device information has been set up
 */
static void writePosition(water_video_arg_position *arg)
{
    // index of sprites starting from x+10
	iowrite16(arg->pos.x, dev.virtbase+10+arg->index*6 );
	iowrite16(arg->pos.y, dev.virtbase+10+arg->index*6+2 );
    iowrite16(arg->type, dev.virtbase+10+arg->index*6+4 );

    dev.argPosition = *arg;
}

static void writeFuel(water_video_arg_fuel *arg)
{
    iowrite16(320-FUELGAUGEHALFLENGTH+arg->fuel, INDICATORX(dev.virtbase) );
    dev.argFuel = *arg;
}

static void writeScore(water_video_arg_score *arg)
{
    short score=arg->score;
    int i;
    for(i=0;i<3;i++){
        iowrite16(score%10, DIGIT1IMG(dev.virtbase)+i*6 );
        score/=10;
    }
    dev.argScore = *arg;
}

static void initBackground(water_video_arg_init *arg){
    iowrite16(arg->scorePos.x, SCOREBOARDX(dev.virtbase));
    iowrite16(arg->scorePos.y, SCOREBOARDY(dev.virtbase));
    iowrite16(arg->digit1Pos.x, DIGIT1X(dev.virtbase));
    iowrite16(arg->digit1Pos.y, DIGIT1Y(dev.virtbase));
    iowrite16(0, DIGIT1IMG(dev.virtbase));
    iowrite16(arg->digit2Pos.x, DIGIT2X(dev.virtbase));
    iowrite16(arg->digit2Pos.y, DIGIT2Y(dev.virtbase));
    iowrite16(0, DIGIT2IMG(dev.virtbase));
    iowrite16(arg->digit3Pos.x, DIGIT3X(dev.virtbase));
    iowrite16(arg->digit3Pos.y, DIGIT3Y(dev.virtbase));
    iowrite16(0, DIGIT3IMG(dev.virtbase));
    iowrite16(arg->fuelPos.x, FUELGAUGEX(dev.virtbase));
    iowrite16(arg->fuelPos.y, FUELGAUGEY(dev.virtbase));
    iowrite16(arg->indicatorPos.x, INDICATORX(dev.virtbase));
    iowrite16(arg->indicatorPos.y, INDICATORY(dev.virtbase));
}

static void playAudio(water_audio_arg *arg){

    iowrite16(1, SHOOTAUDIO(dev.virtbase)+arg->index*2);
}

static void stopAudio(water_audio_arg *arg){

    iowrite16(0, SHOOTAUDIO(dev.virtbase)+arg->index*2);
}

/*
 * Handle ioctl() calls from userspace:
 * Read or write the segments on single digits.
 * Note extensive error checking of arguments
 */
static long water_video_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    water_video_arg_boundary argBoundary;
    water_video_arg_position argPosition;
    water_video_arg_fuel argFuel;
    water_video_arg_score argScore;
    water_video_arg_init argInit;
    water_audio_arg argAudio;

	switch (cmd) {
	// case water_video_WRITE_BACKGROUND:
	// 	if (copy_from_user(&vla, (water_video_arg_t *) arg,
	// 			   sizeof(water_video_arg_t)))
	// 		return -EACCES;
	// 	write_background(&vla.background);
	// 	break;
	
        case WATER_VIDEO_WRITE_BOUNDARY:
            if (copy_from_user(&argBoundary, (water_video_arg_boundary *) arg,
                       sizeof(argBoundary)))
                return -EACCES;
            writeBoundary(&argBoundary);
            break;

        case WATER_VIDEO_WRITE_POSITION:
            if (copy_from_user(&argPosition, (water_video_arg_position *) arg,
                              sizeof(argPosition)))
                return -EACCES;
                writePosition(&argPosition);
                break;
        case WATER_VIDEO_WRITE_FUEL:
            if (copy_from_user(&argFuel, (water_video_arg_fuel *) arg,
                               sizeof(argFuel)))
                return -EACCES;
                writeFuel(&argFuel);
                break;
        case WATER_VIDEO_WRITE_SCORE:
            if (copy_from_user(&argScore, (water_video_arg_score *) arg,
                               sizeof(argScore)))
                return -EACCES;
            writeScore(&argScore);
            break;
        case WATER_VIDEO_INIT:
            if (copy_from_user(&argInit, (water_video_arg_init *) arg,
                               sizeof(argInit)))
                return -EACCES;
            initBackground(&argInit);
            break;
        case WATER_AUDIO_PLAY:
            if (copy_from_user(&argAudio, (water_video_arg_init *) arg,
                               sizeof(argAudio)))
                return -EACCES;
            playAudio(&argAudio);
            break;
        case WATER_AUDIO_STOP:
            if (copy_from_user(&argAudio, (water_video_arg_init *) arg,
                               sizeof(argAudio)))
                return -EACCES;
            stopAudio(&argAudio);
            break;


	// case WATER_VIDEO_READ_BACKGROUND:
	//   	vla.background = dev.background;
	// 	if (copy_to_user((water_video_arg_t *) arg, &vla,
	// 			 sizeof(water_video_arg_t)))
	// 		return -EACCES;
	// 	break;

	default:
		return -EINVAL;
	}

	return 0;
}

/* The operations our device knows how to do */
static const struct file_operations water_video_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl = water_video_ioctl,
};

/* Information about our device for the "misc" framework -- like a char dev */
static struct miscdevice water_video_misc_device = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= DRIVER_NAME,
	.fops		= &water_video_fops,
};

/*
 * Initialization code: get resources (registers) and display
 * a welcome message
 */
static int __init water_video_probe(struct platform_device *pdev)
{
	int ret;

	/* Register ourselves as a misc device: creates /dev/water_video */
	ret = misc_register(&water_video_misc_device);

	/* Get the address of our registers from the device tree */
	ret = of_address_to_resource(pdev->dev.of_node, 0, &dev.res);
	if (ret) {
		ret = -ENOENT;
		goto out_deregister;
	}

	/* Make sure we can use these registers */
	if (request_mem_region(dev.res.start, resource_size(&dev.res),
			       DRIVER_NAME) == NULL) {
		ret = -EBUSY;
		goto out_deregister;
	}

	/* Arrange access to our registers */
	dev.virtbase = of_iomap(pdev->dev.of_node, 0);
	if (dev.virtbase == NULL) {
		ret = -ENOMEM;
		goto out_release_mem_region;
	}

	return 0;

out_release_mem_region:
	release_mem_region(dev.res.start, resource_size(&dev.res));
out_deregister:
	misc_deregister(&water_video_misc_device);
	return ret;
}

/* Clean-up code: release resources */
static int water_video_remove(struct platform_device *pdev)
{
	iounmap(dev.virtbase);
	release_mem_region(dev.res.start, resource_size(&dev.res));
	misc_deregister(&water_video_misc_device);
	return 0;
}

/* Which "compatible" string(s) to search for in the Device Tree */
#ifdef CONFIG_OF
static const struct of_device_id water_video_of_match[] = {
	{ .compatible = "csee4840,vga_ball-1.0" },
	{},
};
MODULE_DEVICE_TABLE(of, water_video_of_match);
#endif

/* Information for registering ourselves as a "platform" driver */
static struct platform_driver water_video_driver = {
	.driver	= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(water_video_of_match),
	},
	.remove	= __exit_p(water_video_remove),
};

/* Called when the module is loaded: set things up */
static int __init water_video_init(void)
{
	pr_info(DRIVER_NAME ": init\n");
	return platform_driver_probe(&water_video_driver, water_video_probe);
}

/* Calball when the module is unloaded: release resources */
static void __exit water_video_exit(void)
{
	platform_driver_unregister(&water_video_driver);
	pr_info(DRIVER_NAME ": exit\n");
}

module_init(water_video_init);
module_exit(water_video_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yongmao Luo, Columbia University");
MODULE_DESCRIPTION("video driver of Water-Raid Project");
