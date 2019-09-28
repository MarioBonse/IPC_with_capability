# KernelProgrammingProject
Repository for course project where we have to implement a simple module for the linux kernel. 
## How to use it
We used [QUEMU](https://www.qemu.org/) for testing the module. 
We made a script sh that compile the module and a script that test it. The script create a .gz file which contains
the module and the script for the test and 
merges this with a filesystem. Finally it launches the QUEMU virtual machine with this filesystem.
```
./creation.sh
```
Then when the virtual machine is ready you can run 
```
sudo /sbin/insmod /Test/*.ko
```
For insert the module and 
```
sudo /sbin/rmmod /Test/*.ko
```
for removing it. 
You can also check the module insered on the virtual machine with
```
sudo /sbin/lsmod
```
