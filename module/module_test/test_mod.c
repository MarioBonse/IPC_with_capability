#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/printk.h>

MODULE_LICENSE("GPL");

static bool cannot_insert;
module_param(cannot_insert, bool, 0);

#define NEW_CAPABILITY        	_IO(WATCHDOG_IOCTL_BASE, 0)
#define WRITE_CAPABILITY        _IOW(WATCHDOG_IOCTL_BASE, 1, int)
#define READ_CAPABILITY     	_IOR(WATCHDOG_IOCTL_BASE, 2, int)
#define DEL_CAPABILITY          _IO(WATCHDOG_IOCTL_BASE, 3)

static int my_ioctl(struct inode *inode, struct file *filp, unsigned int ioctl_num, unsigned long ioctl_param)
{
	switch(ioctl_num)
	{
		case NEW_CAPABILITY:
		printk("new_capability \n");
		
		case WRITE_CAPABILITY:
		printk("write_capability \n");

		case READ_CAPABILITY:
		printk("read_capability \n");

		case DEL_CAPABILITY:
		printk("delete_capability \n");
	}
}

static struct file_operations my_fops = {
  .owner =        THIS_MODULE,
  .unlocked_ioctl =	my_ioctl,
#if 0
  .read =         my_read,
  .open =         my_open,
  .release =      my_close,
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
