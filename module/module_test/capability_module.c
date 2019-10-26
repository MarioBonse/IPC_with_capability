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

//#define DEBUG false
#define DEVICE_NAME "capability_module" //name that will be assigned to misc_device when registering it

MODULE_AUTHOR("Mario Bonsembiante");
MODULE_DESCRIPTION("IPC with capability");
MODULE_LICENSE("GPL");

static struct list_head head;

// !!!
// Managing capability: function that write, read, create, delete a capability
// !!!

struct capability_elem{
    int capability_ID;
    char *message;
    struct mutex my_mutex;
    size_t len;
    struct list_head kl;
};

// Function that check if, given a capability ID it exists.
// It search in the list of capbaility if exists return the corrispondent element in the list
struct capability_elem *check_capability(int capability_ID)
{
    struct list_head *l, *tmp;
    struct capability_elem *n;
    #ifdef DEBUG
    printk("Searching capability:\n");
    #endif

    list_for_each_safe(l, tmp, &head) {
        n = list_entry(l, struct capability_elem, kl);
        if (n->capability_ID == capability_ID){
            #ifdef DEBUG
            printk("this capability exists!\n");
            #endif
            return n;
        }
    }
    #ifdef DEBUG
    printk("this capability does not exists!\n");
    #endif
    return NULL;
}




/* function that create a new capability.
It creates an element capability_elem, init it and add it in the queue */
int new_capability(void)
{
  struct capability_elem  *new_elem, *test;
  new_elem = kmalloc(sizeof(struct capability_elem), GFP_KERNEL);
  // we have to check if the id was assigned to another capability before
  do{
    new_elem->capability_ID = get_random_int();
    test = check_capability(new_elem->capability_ID);
  }while( test != NULL);
  
  new_elem->len = 0;
  new_elem->message = NULL;
  // init the mutex
  mutex_init(&(new_elem->my_mutex));
  // ad to the list of capability
  #ifdef DEBUG
	printk("NEW CAPABILITY CREATED");
  #endif
  list_add(&(new_elem->kl), &head);
	return new_elem->capability_ID;
}




size_t write_capability(int capability_ID, char __user *buf, size_t len)
{
    struct capability_elem *my_capability;
    my_capability = check_capability(capability_ID);
    if (my_capability == NULL)
    {
        #ifdef DEBUG
        printk("ERROR: this capability does not exists");
        #endif
        return 0;
    }else{
         // we can write into the capability
            int err;
            size_t count;
            count = len;
            #ifdef DEBUG
            printk("The capability exists, now we can WRITE");
            printk("writing %ld bytes.", len);
            #endif

            mutex_lock(&my_capability->my_mutex);
            if (my_capability->message) {
              // there is somenthing writtne on the capability. 
              // now it's not blocking
              #ifdef DEBUG
              printk("Error someone has already written");
              #endif
              count = -1;
              goto exit;
            }
            my_capability->message = kmalloc(count, GFP_USER);
            if (my_capability->message == NULL) {
              #ifdef DEBUG
              printk("Error during memory allocation");
              #endif
              count = -1;
              goto exit;
            }

            err = copy_from_user(my_capability->message, buf, count);
            #ifdef DEBUG
            printk("COPIED %s", my_capability->message);
            #endif
            if (err) {
              #ifdef DEBUG
              printk("Error during copy");
              #endif
              count = -EFAULT;
              goto exit;
            }
            my_capability->len = count;
            exit:
            mutex_unlock(&my_capability->my_mutex);

            return count;
            }
}


ssize_t read_capability(int capability_ID, char __user *buf, size_t len)
{
    int res, err;
    struct capability_elem *my_capability;
    my_capability = check_capability(capability_ID);
    if (my_capability == NULL)
    {
        #ifdef DEBUG
        printk("ERROR: this capability does not exists");
        #endif
        return 0;
    }else{
        #ifdef DEBUG
		    printk("The capability exists, now we can READ");
        printk("Reading %ld bytes.", len);        
        #endif

        // we can read from the capability
        mutex_lock(&my_capability->my_mutex);
        if (len > my_capability->len) {
          res = my_capability->len;
        } else {
          res = len;
        }
        if (my_capability->message == NULL) {
			res = 0;
			goto exit_read;
        }
		err = copy_to_user(buf, my_capability->message, res);
		if (err) {
			res = -EFAULT;
			goto exit_read;
		}
		kfree(my_capability->message);
		my_capability->message = NULL;
		exit_read:
		mutex_unlock(&my_capability->my_mutex);

		return res;
	}
}

int del_capability(int capability_ID){
	    struct list_head *l, *tmp;
    struct capability_elem *n;
    #ifdef DEBUG
    printk("Searching capability:\n");
    #endif

    list_for_each_safe(l, tmp, &head) {
        n = list_entry(l, struct capability_elem, kl);
        if (n->capability_ID == capability_ID){
            #ifdef DEBUG
            printk("I can delete this capability\n");
            #endif
            list_del(l);
        	kfree(n);
			return 1;
        }
    }
    #ifdef DEBUG
    printk("this capability does not exists!\n");
    #endif
            
    return 0;
}

static int my_open(struct inode *inode, struct file *filp)
{
  #ifdef DEBUG
  printk(KERN_INFO "Inside open \n");
  #endif
  return 0;
}

static int my_close(struct inode *inode, struct file *filp) {
  #ifdef DEBUG
  printk (KERN_INFO "Inside close \n");
  #endif
  return 0;
}

static long my_ioctl(
                    struct file *filp, 
                    unsigned int cmd, 
                    unsigned long argp)
{
	void __user *arg_user = (void __user *)argp;
	// arg_user = (void __user *)argp;
	struct ioctl_message *message = arg_user;
  #ifdef DEBUG
  pr_info("cmd = %x\n", cmd);
  #endif
	switch(cmd)
	{
		case NEW_CAPABILITY:
		return new_capability();
		break;
		
		case WRITE_CAPABILITY:
		return write_capability(message->capability, message->buff, message->len);
		break;

		case READ_CAPABILITY:
		return read_capability(message->capability, message->buff, message->len);
    break;

		case DEL_CAPABILITY:
		return del_capability(message->capability);
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
	.mode	= 0777,	// this should set the permissions
};


static int __init testmodule_init(void)
{
  int res;
  res = misc_register(&test_device);
  #ifdef DEBUG
  printk("DEBUG MODE\n\n");
  #endif
  printk("Misc Register returned %d\n", res);
  INIT_LIST_HEAD(&head);


  return 0;
}

static void __exit testmodule_exit(void)
{
  misc_deregister(&test_device);
}

module_init(testmodule_init);
module_exit(testmodule_exit);








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


