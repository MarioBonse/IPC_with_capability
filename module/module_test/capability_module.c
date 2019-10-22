#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/ioctl.h> 
#include <linux/printk.h>
#include <linux/random.h>
#include <linux/list.h>

#include "ioctl.h"

#define DEVICE_NAME "capability_module" //name that will be assigned to misc_device when registering it

MODULE_AUTHOR("Mario Bonsembiante");
MODULE_DESCRIPTION("IPC with capability");
MODULE_LICENSE("GPL");

static struct list_head head;

static int my_open(struct inode *inode, struct file *filp)
{
 printk(KERN_INFO "Inside open \n");
 return 0;
}

static int my_close(struct inode *inode, struct file *filp) {
 printk (KERN_INFO "Inside close \n");
 return 0;
}

static long my_ioctl(
                    struct file *filp, 
                    unsigned int cmd, 
                    unsigned long argp)
{
  void __user *arg_user;
  arg_user = (void __user *)argp;
  pr_info("cmd = %x\n", cmd);
	switch(cmd)
	{
		case NEW_CAPABILITY:
		printk("Executing new_capability\n");
		break;
		
		case WRITE_CAPABILITY:
		printk("Executing write_capability\n");
		break;

		case READ_CAPABILITY:
		printk("Executing read_capability\n");
		break;

		case DEL_CAPABILITY:
		printk("Executing delete_capability\n");
		break;
	}
	return 0;
}

static struct file_operations my_fops = {
  .owner          = THIS_MODULE,
  .unlocked_ioctl = my_ioctl,
  .open           = my_open,
  .release        = my_close,
};


static struct miscdevice test_device = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVICE_NAME,
	.fops	= &my_fops,
	.mode	= 0777,	// this should the permissions
};


static int testmodule_init(void)
{
  int res;
  res = misc_register(&test_device);

  printk("Misc Register returned %d\n", res);

  return 0;
}

static void testmodule_exit(void)
{
  misc_deregister(&test_device);
}

module_init(testmodule_init);
module_exit(testmodule_exit);

/*
!!!
Managing capability: function that write, read, create, delete a capability
!!!
*/
// struct capability_elem{
//     int capability_ID;
//     char *message;
//     struct mutex my_mutex;
//     size_t len;
//     struct list_head kl;
// };
// /* function that create a new capability.
// It creates an element capability_elem, init it and add it in the queue */
// int new_capability(void)
// {
//     struct capability_elem  *new_elem;
//     new_elem = kmalloc(sizeof(struct capability_elem), GFP_KERNEL);
//     new_elem->capability_ID = get_random_int();
//     new_elem->len = 0;
//     // init the mutex
//     mutex_init(&(new_elem->my_mutex));
//     // ad to the list of capability
//     list_add(&(new_elem->kl), &head);
// }

// /*
// Function that check if, given a capability ID it exists.
// It search in the list of capbaility if exists return the corrispondent element in the list
//  */
// struct capability_elem *check_capability(int capability_ID)
// {
//     struct list_head *l, *tmp;
//     struct capability_elem *n;
//     printk("Searching capability:\n");

//     list_for_each_safe(l, tmp, &head) {
//         n = list_entry(l, struct node, kl);
//         if (n->capability_ID == capability_ID){
//             printk("this capability exists!\n");
//             return n;
//         }
//     }
//     printk("this capability does not exists!\n");
//     return NULL
// }

// ssize_t read_capability(int capability_ID, char __user *buf, size_t len)
// {
//     printk("So long, and thanks for all the fish!!! Your friendly test module is going to be removed from the kernel\n");

//     misc_deregister(%test_device);
//     int res, err;
//     struct capability_elem *my_capability;
//     my_capability = check_capability(capability_ID);
//     if (my_capability == NULL)
//     {
//         printk("ERROR: this capability does not exists");
//         return 0;
//     }else{
//         // we can read from the capability
//         mutex_lock(&my_capability->my_mutex);
//         if (len > my_capability->len) {
//           res = my_capability->len;
//         } else {
//           res = len;
//         }
//         if (my_capability->message == NULL) {
//         mutex_unlock(&my_capability->my_mutex);
//         return 0;
//         }
//       err = copy_to_user(buf, my_capability->message, res);
//       if (err) {
//       return -EFAULT;
//       }

//       kfree(my_capability->message);
//       my_capability->message = NULL;
//       mutex_unlock(&my_capability->my_mutex);

//       return res;
// }

// size_t write_capability(int capability_ID, char __user *buf, size_t len)
// {
//     struct capability_elem *my_capability;
//     my_capability = check_capability(capability_ID);
//     if (my_capability == NULL)
//     {
//         printk("ERROR: this capability does not exists");
//         return 0;
//     }else{
//          // we can write into the capability
//             int err;
//             size_t count;
//             count = len;

//             mutex_lock(&my_capability->my_mutex);
//             if (my_capability->message) {
//               // there is somenthing writtne on the capability. 
//               // now it's not blocking
//               printk("Error someone has already written");
//               count = -1;
//               goto exit;
//             }
//             my_capability->message = kmalloc(count, GFP_USER);
//             if (my_capability->message == NULL) {
//               printk("Error during memory allocation");
//               count = -1;
//               goto exit;
//             }

//             err = copy_from_user(my_capability->message, buf, count);
//             if (err) {
//               printk("Error during copy");
//               count = -EFAULT;
//               goto exit;
//             }
//             exit:
//             mutex_unlock(&my_capability->my_mutex);

//             return count;
//             }
// }


