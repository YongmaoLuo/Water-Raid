/* * Device driver for the VGA video generator
 *
 * A Platform device implemented using the misc subsystem
 *
 * Stephen A. Edwards
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
 * Check code style with
 * checkpatch.pl --file --no-tree water_video.c
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

#define DRIVER_NAME "vga_ball"

/* Device registers */
// #define BG_RED(x) (x)
// #define BG_GREEN(x) ((x)+1)
// #define BG_BLUE(x) ((x)+2)

#define BOUNDARY0(x) (x)
#define BOUNDARY1(x) ((x)+2)
#define BOUNDARY2(x) ((x)+4)
#define BOUNDARY3(x) ((x)+6)
#define SHIFT(x) ((x)+8)
// index of sprites starting from x+8


/*
 * Information about our device
 */
struct water_video_dev {
	struct resource res; /* Resource: our registers */
	void __iomem *virtbase; /* Where registers can be accessed in memory */
  	water_video_arg_boundary argBoundary;
      water_video_arg_position argPosition;
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

static void write_boundary(water_video_arg_boundary *arg)
{
    iowrite16(arg->boundary.river1_left, BOUNDARY0(dev.virtbase) );
    iowrite16(arg->boundary.river1_right, BOUNDARY1(dev.virtbase) );
    iowrite16(arg->boundary.river2_left, BOUNDARY2(dev.virtbase) );
    iowrite16(arg->boundary.river2_right, BOUNDARY3(dev.virtbase) );
    iowrite8(arg->shift, SHIFT(dev.virtbase));
    dev.argBoundary = *arg;
}

/*
 * Write 16 bits for each location variable
 * Assumes digit is in range and the device information has been set up
 */
static void write_position(water_video_arg_position *arg)
{
	iowrite16(arg->pos.x, dev.virtbase+10+arg->index*6 );
	iowrite16(arg->pos.y, dev.virtbase+10+arg->index*6+2 );
    iowrite16(arg->type, dev.virtbase+10+arg->index*6+4 );

    dev.argPosition = *arg;
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
		write_boundary(&argBoundary);
		break;

	case WATER_VIDEO_WRITE_POSITION:
        if (copy_from_user(&argPosition, (water_video_arg_position *) arg,
                          sizeof(argPosition)))
            return -EACCES;
            write_position(&argPosition);
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
