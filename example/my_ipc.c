#include <linux/poll.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/wait.h>
#include <linux/list.h>

MODULE_AUTHOR("Luca Abeni");
MODULE_DESCRIPTION("Simple and minimalistic IPC based on misc devices and lists");
MODULE_LICENSE("GPL");

struct node {
  char *pointer;
  int len;
  struct list_head kl;
};

struct list_head head;
static struct mutex my_mutex;
static wait_queue_head_t my_waitqueue;

static int my_open(struct inode *inode, struct file *file)
{
  int *count;

  count = kmalloc(sizeof(int), GFP_USER);
  if (count == NULL) {
    return -1;
  }

  *count = 0;
  file->private_data = count;
  
  return 0;
}

static int my_close(struct inode *inode, struct file *file)
{
  kfree(file->private_data);
  return 0;
}

ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *ppos)
{
  int res, err;
  int *count;
  struct node *n;

  count = file->private_data;
  if (*count == 1) {
    return 0;
  }
  mutex_lock(&my_mutex);
  while (list_empty(&head)) {
    mutex_unlock(&my_mutex);
    if (wait_event_interruptible(my_waitqueue, !list_empty(&head))) {
      return -ERESTARTSYS;
    }
    mutex_lock(&my_mutex);
  }
  n = list_first_entry(&head, struct node, kl);

  if (len > n->len) {
    res = n->len;
  } else {
    res = len;
  }
  err = copy_to_user(buf, n->pointer, res);
  if (err) {
    mutex_unlock(&my_mutex);
    return -EFAULT;
  }

  kfree(n->pointer);
  list_del(&n->kl);
  kfree(n);
  *count = 1;
  mutex_unlock(&my_mutex);

  return res;
}

static ssize_t my_write(struct file *file, const char __user * buf, size_t count, loff_t *ppos)
{
  int err;
  struct node *n;

  mutex_lock(&my_mutex);
  n = kmalloc(sizeof(struct node), GFP_USER);
  if (n == NULL) {
    mutex_unlock(&my_mutex);

    return -1;
  }
  n->pointer = kmalloc(count, GFP_USER);
  if (n->pointer == NULL) {
    kfree(n);
    mutex_unlock(&my_mutex);

    return -1;
  }
  n->len = count;

  list_add_tail(&(n->kl), &head);

  err = copy_from_user(n->pointer, buf, count);
  if (err) {
    mutex_unlock(&my_mutex);

    return -EFAULT;
  }

  wake_up_interruptible(&my_waitqueue);
  mutex_unlock(&my_mutex);

  return count;
}

static struct file_operations my_fops = {
  .owner =        THIS_MODULE,
  .read =         my_read,
  .open =         my_open,
  .release =      my_close,
  .write =        my_write,
};

static struct miscdevice test_device = {
  MISC_DYNAMIC_MINOR, "test", &my_fops
};


static int testmodule_init(void)
{
  int res;

  INIT_LIST_HEAD(&head);
  res = misc_register(&test_device);

  printk("Misc Register returned %d\n", res);

  mutex_init(&my_mutex);
  init_waitqueue_head(&my_waitqueue);

  return 0;
}

static void testmodule_exit(void)
{
  misc_deregister(&test_device);
}

module_init(testmodule_init);
module_exit(testmodule_exit);
