#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
//#include <linux/ioctl.h>
#include <linux/printk.h>

#include "../ioctl.h"

MODULE_LICENSE("GPL");

static int Major;

static bool cannot_insert;
module_param(cannot_insert, bool, 0);

static int my_open(struct inode *inode, struct file *filp)
{
 printk(KERN_INFO "Inside open \n");
 return 0;
}

static int my_close(struct inode *inode, struct file *filp) {
 printk (KERN_INFO "Inside close \n");
 return 0;
}

static int my_ioctl(struct inode *inode, struct file *filp, unsigned int ioctl_num, unsigned long ioctl_param)
{
	switch(ioctl_num)
	{
		case NEW_CAPABILITY:
		printk("new_capability sku sku\n");
		
		case WRITE_CAPABILITY:
		printk("write_capability \n");

		case READ_CAPABILITY:
		printk("read_capability \n");

		case DEL_CAPABILITY:
		printk("delete_capability \n");
	}
	return 0;
}

static struct file_operations my_fops = {
  .owner =        	THIS_MODULE,
  .unlocked_ioctl =	my_ioctl,
  .open =         	my_open,
  .release =      	my_close,
#if 0
  .read =         my_read,
  .write =        my_write,
  .poll =         my_poll,
  .fasync =       my_fasync,
#endif
};


static struct miscdevice test_device = {
  MISC_DYNAMIC_MINOR, "test", &my_fops
};

static int __init test_module_init(void)
{
    printk("Hi there!!! This is your friendly test module, linked in the kernel\n");

    int res;
    res = misc_register(&test_device);

    if (cannot_insert) {
        return -1;
    }
    return 0;
}

static void __exit test_module_unload(void)
{
    printk("So long, and thanks for all the fish!!! Your friendly test module is going to be removed from the kernel\n");

    misc_deregister(%test_device);
}

module_init(test_module_init);
module_exit(test_module_unload);
