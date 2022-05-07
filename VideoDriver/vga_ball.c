/* * Device driver for the VGA video generator
 *
 * A Platform device implemented using the misc subsystem
 *
 * Stephen A. Edwards
 * Columbia University
 *
 * References:
 * Linux source: Documentation/driver-model/platform.txt
et_modulowrite16(background_struct->sprite3img, SPRITE3IMG(dev.virtbase));
e_assignment embeddedsw.dts.vendor "csee4840"
set_module_assignment embeddedsw.dts.name "vga_ball"
set_module_assignment embeddedsw.dts.group "vga"*               drivers/misc/arm-charlcd.c
 * http://www.linuxforu.com/tag/linux-device-drivers/
 * http://free-electrons.com/docs/
 *
 * "make" to build
 * insmod vga_ball.ko
 *
 * Check code style with
 * checkpatch.pl --file --no-tree vga_ball.c
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
#include "vga_ball.h"

#define DRIVER_NAME "vga_ball"

/* Device registers */
#define BOUND1(x) (x)
#define BOUND2(x) ((x)+2)
#define BOUND3(x) ((x)+4)
#define BOUND4(x) ((x)+6)
#define SHIFT(x) ((x)+8)
#define SPRITE1X(x) ((x)+10)
#define SPRITE1Y(x) ((x)+12)
#define SPRITE1IMG(x) ((x)+14)
#define SPRITE2X(x) ((x)+16)
#define SPRITE2Y(x) ((x)+18)
#define SPRITE2IMG(x) ((x)+20)
#define SPRITE3X(x) ((x)+22)
#define SPRITE3Y(x) ((x)+24)
#define SPRITE3IMG(x) ((x)+26)
#define FUELX(x) ((x) + 28)
#define FUELY(x) ((x) + 30)
#define SPRITE4IMG(x) ((x) + 32)
#define SPRITE5X(x) ((x) + 34)
#define SPRITE5Y(x) ((x) + 36)
#define SPRITE5IMG(x) ((x) + 48)
#define SPRITE6X(x) ((x) + 40)
#define SPRITE6Y(x) ((x) + 42)
#define SPRITE6IMG(x) ((x) + 44)
#define SPRITE7X(x) ((x) + 46)
#define SPRITE7Y(x) ((x) + 48)
#define SPRITE7IMG(x) ((x) + 50)
#define SPRITE8X(x) ((x) + 52)
#define SPRITE8Y(x) ((x) + 54)
#define SPRITE8IMG(x) ((x) + 56)
#define SPRITE9X(x) ((x) + 58)
#define SPRITE9Y(x) ((x) + 60)
#define SPRITE9IMG(x) ((x) + 62)
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

/*
 * Information about our device
 */
struct vga_ball_dev {
	struct resource res; /* Resource: our registers */
	void __iomem *virtbase; /* Where registers can be accessed in memory */
} dev;

/*
 * Write segments of a single digit
 * Assumes digit is in range and the device information has been set up
 */
static void write_background(vga_ball_arg_t *background_struct)
{
	iowrite16(background_struct->bound1, BOUND1(dev.virtbase) );
	iowrite16(background_struct->bound2, BOUND2(dev.virtbase) );
	iowrite16(background_struct->bound3, BOUND3(dev.virtbase) );
	iowrite16(background_struct->bound4, BOUND4(dev.virtbase));
        iowrite16(background_struct->shift, SHIFT(dev.virtbase));
	iowrite16(background_struct->sprite1x, SPRITE1X(dev.virtbase));
	iowrite16(background_struct->sprite1y, SPRITE1Y(dev.virtbase));
	iowrite16(background_struct->sprite1img, SPRITE1IMG(dev.virtbase));
	iowrite16(background_struct->sprite2x, SPRITE2X(dev.virtbase));
	iowrite16(background_struct->sprite2y, SPRITE2Y(dev.virtbase));
	iowrite16(background_struct->sprite2img, SPRITE2IMG(dev.virtbase));
        iowrite16(background_struct->sprite3x, SPRITE3X(dev.virtbase));
        iowrite16(background_struct->sprite3y, SPRITE3Y(dev.virtbase));
        iowrite16(background_struct->sprite3img, SPRITE3IMG(dev.virtbase));
        iowrite16(background_struct->fuelx, FUELX(dev.virtbase));
        iowrite16(background_struct->fuely, FUELY(dev.virtbase));
        iowrite16(background_struct->sprite4img, SPRITE4IMG(dev.virtbase));
        iowrite16(background_struct->scoreboardx, SCOREBOARDX(dev.virtbase));
        iowrite16(background_struct->scoreboardy, SCOREBOARDY(dev.virtbase));
        iowrite16(background_struct->digit1x, DIGIT1X(dev.virtbase));
        iowrite16(background_struct->digit1y, DIGIT1Y(dev.virtbase));
        iowrite16(background_struct->digit1img, DIGIT1IMG(dev.virtbase));
        iowrite16(background_struct->digit2x, DIGIT2X(dev.virtbase));
        iowrite16(background_struct->digit2y, DIGIT2Y(dev.virtbase));
        iowrite16(background_struct->digit2img, DIGIT2IMG(dev.virtbase));
        iowrite16(background_struct->digit3x, DIGIT3X(dev.virtbase));
        iowrite16(background_struct->digit3y, DIGIT3Y(dev.virtbase));
        iowrite16(background_struct->digit3img, DIGIT3IMG(dev.virtbase));
	iowrite16(background_struct->fuelgaugex, FUELGAUGEX(dev.virtbase));
	iowrite16(background_struct->fuelgaugey, FUELGAUGEY(dev.virtbase));
        iowrite16(background_struct->indicatorx, INDICATORX(dev.virtbase));
        iowrite16(background_struct->indicatory, INDICATORY(dev.virtbase));
	iowrite16(background_struct->sprite5x, SPRITE5X(dev.virtbase));
        iowrite16(background_struct->sprite5y, SPRITE5Y(dev.virtbase));
        iowrite16(background_struct->sprite5img, SPRITE5IMG(dev.virtbase));
        iowrite16(background_struct->sprite6x, SPRITE6X(dev.virtbase));
        iowrite16(background_struct->sprite6y, SPRITE6Y(dev.virtbase));
        iowrite16(background_struct->sprite6img, SPRITE6IMG(dev.virtbase));
        iowrite16(background_struct->sprite7x, SPRITE7X(dev.virtbase));
        iowrite16(background_struct->sprite7y, SPRITE7Y(dev.virtbase));
        iowrite16(background_struct->sprite7img, SPRITE7IMG(dev.virtbase));
        iowrite16(background_struct->sprite8x, SPRITE8X(dev.virtbase));
        iowrite16(background_struct->sprite8y, SPRITE8Y(dev.virtbase));
        iowrite16(background_struct->sprite8img, SPRITE8IMG(dev.virtbase));
        iowrite16(background_struct->sprite9x, SPRITE9X(dev.virtbase));
        iowrite16(background_struct->sprite9y, SPRITE9Y(dev.virtbase));
        iowrite16(background_struct->sprite9img, SPRITE9IMG(dev.virtbase));


}

/*
 * Handle ioctl() calls from userspace:
 * Read or write the segments on single digits.
 * Note extensive error checking of arguments
 */
static long vga_ball_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	vga_ball_arg_t vla;

	switch (cmd) {
	case VGA_BALL_WRITE_BACKGROUND:
		if (copy_from_user(&vla, (vga_ball_arg_t *) arg,
				   sizeof(vga_ball_arg_t)))
			return -EACCES;
		write_background(&vla);
		break;

	case VGA_BALL_READ_BACKGROUND:
		if (copy_to_user((vga_ball_arg_t *) arg, &vla,
				 sizeof(vga_ball_arg_t)))
			return -EACCES;
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

/* The operations our device knows how to do */
static const struct file_operations vga_ball_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl = vga_ball_ioctl,
};

/* Information about our device for the "misc" framework -- like a char dev */
static struct miscdevice vga_ball_misc_device = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= DRIVER_NAME,
	.fops		= &vga_ball_fops,
};

/*
 * Initialization code: get resources (registers) and display
 * a welcome message
 */
static int __init vga_ball_probe(struct platform_device *pdev)
{
	vga_ball_arg_t arg;
	arg.bound1 = 100;
	arg.bound2 = 200;
	arg.bound3 = 0;
	arg.bound4 = 0;
	arg.shift = 0;
	arg.sprite1x = 100;
	arg.sprite1y = 201;
	arg.sprite1img = 0;
	arg.sprite2x = 400;
	arg.sprite2y = 401;
	arg.sprite2img = 1;
	arg.sprite3x = 200;
	arg.sprite3y = 401;
	arg.sprite3img = 2;
	arg.fuelx = 400;
	arg.fuely = 400;
	arg.sprite4img = 3;
	int ret;

	/* Register ourselves as a misc device: creates /dev/vga_ball */
	ret = misc_register(&vga_ball_misc_device);

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
        
	/* Set an initial color */
        write_background(&arg);

	return 0;

out_release_mem_region:
	release_mem_region(dev.res.start, resource_size(&dev.res));
out_deregister:
	misc_deregister(&vga_ball_misc_device);
	return ret;
}

/* Clean-up code: release resources */
static int vga_ball_remove(struct platform_device *pdev)
{
	iounmap(dev.virtbase);
	release_mem_region(dev.res.start, resource_size(&dev.res));
	misc_deregister(&vga_ball_misc_device);
	return 0;
}

/* Which "compatible" string(s) to search for in the Device Tree */
#ifdef CONFIG_OF
static const struct of_device_id vga_ball_of_match[] = {
	{ .compatible = "csee4840,vga_ball-1.0" },
	{},
};
MODULE_DEVICE_TABLE(of, vga_ball_of_match);
#endif

/* Information for registering ourselves as a "platform" driver */
static struct platform_driver vga_ball_driver = {
	.driver	= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(vga_ball_of_match),
	},
	.remove	= __exit_p(vga_ball_remove),
};

/* Called when the module is loaded: set things up */
static int __init vga_ball_init(void)
{
	pr_info(DRIVER_NAME ": init\n");
	return platform_driver_probe(&vga_ball_driver, vga_ball_probe);
}

/* Calball when the module is unloaded: release resources */
static void __exit vga_ball_exit(void)
{
	platform_driver_unregister(&vga_ball_driver);
	pr_info(DRIVER_NAME ": exit\n");
}

module_init(vga_ball_init);
module_exit(vga_ball_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stephen A. Edwards, Columbia University");
MODULE_DESCRIPTION("VGA ball driver");
