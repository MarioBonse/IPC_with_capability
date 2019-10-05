#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/wait.h>

MODULE_AUTHOR("Mario Bonsembiante Francesco Iori");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A very simple module for synchronous IPC with kapability");
MODULE_VERSION("0.1");

// We use list for implementing the kapability. We try to write and read to a particular kapability
// with a certain ID. 
// we can allocate a capability with allocate wich return a 64 bit key
// and then read and write only to a specific kapability
struct node {
    uint64_t int;
    char *pointer;
    int len;
    struct list_head kl;
};

struct list_head head;
static struct mutex my_mutex;
static wait_queue_head_t my_waitqueue;

static bool cannot_insert;
module_param(cannot_insert, bool, 0);

static int __init test_module_init(void)
{
    printk("Hi there!!! This is your friendly test module, linked in the kernel\n");

    if (cannot_insert) {
        return -1;
    }
    return 0;
}

static void __exit test_module_unload(void)
{
    printk("So long, and thanks for all the fish!!! Your friendly test module is going to be removed from the kernel\n");
}

module_init(test_module_init);
module_exit(test_module_unload);
