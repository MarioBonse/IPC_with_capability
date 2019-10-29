# IPC with capability
On this educational project we implemented a linux module for inter process communication(IPC) with capability. 

A [capability](https://en.wikipedia.org/wiki/Capability-based_security) (known in some systems as a key) is a communicable, unforgeable token of authority. It refers to a value that references an object along with an associated set of access rights. 
Every capability represent a different communication channel, with a unique key. 

A process will be able to read and write a message in a capability channel if and only if it has the token (and so the right) associate with the communication channel. We decide to implement an API which let the user to _create_, _read_, _write_ and _delete_ a capability. 

    1. *key* = *create()* function for creating a capability. It will return a value for the capability which has to be used in order to \textit{read/write} in the channel
    2. *write(message,key)*   Function for writing a message given a key
    3. *message* = *read(key)* Function for reading a message given a key
    4. *delete(key)* Function for deleting a capability channel

We decided to implement asynchronous communication, so a process will not be block if there isn't something to read or there already a message in a capability channel. Implementing different policy is very easy.

## Managing capability
In order to manage different capabilities channels we used the kernel lists. Each capability communication channel is represented by a structure. Each communication channel has its own mutex, in order to avoid concurrent access to the buffer, a key, a pointer to a buffer (in kernel space), the lenght of the messagge and a pointer to the next element of the list. We can see now the _struct_ that represents the the capability communication channel.

```C
struct capability_elem{
    int key;
    char *message;
    struct mutex my_mutex;
    size_t len;
    struct list_head kl;
};
```

## IOCTL
In order to manage the different functionality needed for the capability we couldn't rely only on the _write_ and _read_ mechanism associated with a miscellaneous device, so we implemented _ioctl_ functionality into the module. 
The associated my\_ioctl() function was associated to the correct operation in the _struct file\_operations_

First, we added the ioctl function to the file operations and implemented the function _my_ioctl()_ to manage the different call through a switch with the different call that are available for the ioctl. 
Example of the basic structure for managing ioctl:

```C
static long my_ioctl(struct file *filp, unsigned int cmd, unsigned long argp)
{
	void __user *arg_user = (void __user *)argp;

	struct ioctl_message *message = arg_user;

	switch(cmd)
	{
		case NEW_CAPABILITY:
		new_capability();
		break;
        
        .
        .
        .
	}
	return 0;
}
```

The different cases of the switch where then defined in an header file, ioctl.h, that need to be included in both the code for the kernel module and the user executable. 
In this same header is defined the _struct ioctl\_message_ used as an input for the various ioctl calls.


```C
#define MY_IOC_MAGIC_NUMBER 10 

#define NEW_CAPABILITY        	_IO(MY_IOC_MAGIC_NUMBER, 0)
#define WRITE_CAPABILITY        _IOW(MY_IOC_MAGIC_NUMBER, 1, struct ioctl_message*)
#define READ_CAPABILITY     	_IOR(MY_IOC_MAGIC_NUMBER, 2, struct ioctl_message*)
#define DEL_CAPABILITY          _IOW(MY_IOC_MAGIC_NUMBER, 3, struct ioctl_message*)
```

File permissions were set through the member _mode_ of the struct _misc_device_, to allow reading, writing, & executing for owner, group and other, to be sure that the user application could open the file and do operations on it.


## How to use it
We used [QUEMU](https://www.qemu.org/) for testing the module. 
We made a script sh that compile the module and the script that test it. The script creates a .gz file which contains
the module and the script for the test it and 
merges this with a filesystem. Finally it launches the QUEMU virtual machine with this filesystem.
In order to use this scritp you have to compile a kernel in a directory called it `kernel`.
Then you can launch the script with
```
./creation.sh
```
In a few seconds everithing is compiled and the VM will be ready. Now you can insert the module with:
```
sudo /sbin/insmod /Test/capabilty_module.ko
```
Then you can test the script with 
```
sudo /Test/test_user_exe
```
You can always in general remove the module
```
sudo /sbin/rmmod /Test/capabilty_module.ko
```
and also check that the module has been inserted in the kernel (running on the virtual machine) with
```
sudo /sbin/lsmod
```
and finally show the kernel'S log with
```
sudo /sbin/dmesg
```

## Authors

* **Mario Bonsembiante** :nerd_face: - [MarioBonse](https://github.com/MarioBonse)
* **Francesco Iori** :nerd_face: - [fraiori0](https://github.com/fraiori0)

