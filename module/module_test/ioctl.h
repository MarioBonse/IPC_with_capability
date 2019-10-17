#ifndef IOCTL_H
#define IOCTL_H

#include <linux/ioctl.h>

#define MY_IOC_MAGIC_NUMBER 345 // va assegnato così brute force? 

#define NEW_CAPABILITY        	_IO(MY_IOC_MAGIC_NUMBER, 0, null)
#define WRITE_CAPABILITY        _IOW(MY_IOC_MAGIC_NUMBER, 1, int)
#define READ_CAPABILITY     	_IOR(MY_IOC_MAGIC_NUMBER, 2, int)
#define DEL_CAPABILITY          _IO(MY_IOC_MAGIC_NUMBER, 3, null)

struct ioctl_message{
    int capability;
    char *buff;
};
#endif

