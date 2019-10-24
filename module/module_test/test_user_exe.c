#include <fcntl.h>      /* open */ 
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <stdio.h>
#include <stdlib.h>

#include "ioctl.h"

#define DEVICE_NAME "/dev/capability_module"  /* "capability_module" è il nome che viene assegnato al misc device quando viene registrato */


/* Functions for the ioctl calls */

int ioctl_new_capability(int file_desc)
{
  int capability;
  capability = ioctl(file_desc, NEW_CAPABILITY, NULL);
  if (capability < 0) {
    printf ("ioctl_set_msg failed:%d\n", capability);
  }
  return capability;
}

int ioctl_read_capability(int file_desc, int capability, char*buff, int len)
{
  int ret;
  struct ioctl_message *messagge = malloc(sizeof(struct ioctl_message));
  messagge->capability = capability;
  messagge->buff = buff;
  messagge->len = len;
  ret = ioctl(file_desc, READ_CAPABILITY, messagge);
  // copy messagge.buff in messagge
  if (ret < 0) {
    printf ("ioctl_read_msg failed:%d\n", ret);
    exit(-1);
  }
  return ret;
}

int ioctl_write_capability(int file_desc, int capability, char*buff, int len)
{
  int ret;
  struct ioctl_message *messagge = malloc(sizeof(struct ioctl_message));
  messagge->capability = capability;
  messagge->buff = buff;
  messagge->len = len;
  ret = ioctl(file_desc, WRITE_CAPABILITY, messagge);
  // copy messagge.buff in messagge
  if (ret < 0) {
    printf ("ioctl_read_msg failed:%d\n", ret);
    exit(-1);
  }
  return ret;
}

/* Main - Call the ioctl functions */
int main()
{
  char *message;
  int len = 2;
  int capability;
  char text[]="ciao";
  int file_desc;
  printf("\nScript loaded, now we will try to open the device\n");
  file_desc = open(DEVICE_NAME, O_RDWR);
  if (file_desc == -1) {
     perror("OPEN");
     printf ("Can't open device file : %s\n", 
             DEVICE_NAME);
     exit(-1);
  }
  capability = ioctl_new_capability(file_desc);
  len = ioctl_write_capability(file_desc, capability, text, sizeof(text));
  len = ioctl_read_capability(file_desc, capability, message, sizeof(text));
  printf("%s", message); 
  printf("ioctl executed\n");
  close(file_desc); 
}


