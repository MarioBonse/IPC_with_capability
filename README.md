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
In order to implemet system call we exploted the IOCTL mechanism.


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
You can always in generale remove the module
```
sudo /sbin/rmmod /Test/capabilty_module.ko
```
and also check the modules insered on the virtual machine with
```
sudo /sbin/lsmod
```
and finally shows the kernel'S log with
```
sudo /sbin/dmesg
```

## Authors

* **Mario Bonsembiante** :nerd_face: - [MarioBonse](https://github.com/MarioBonse)
* **Francesco Iori** :nerd_face: - [fraiori0](https://github.com/fraiori0)

