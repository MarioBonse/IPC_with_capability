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
  capability = ioctl(file_desc, NEW_CAPABILITY);
  if (capability < 0) {
    printf ("ioctl_set_msg failed:%d\n", capability);
  }
  return capability;
}

int ioctl_send_capability(int file_desc, int capability, char*buff)
{
  int len;
  struct ioctl_message messagge;
  messagge.capability = capability;
  messagge.buff = buff;
  len = ioctl(file_desc, WRITE_CAPABILITY, messagge);
  // copy messagge.buff in messagge
  if (capability < 0) {
    printf ("ioctl_set_msg failed:%d\n", len);
    exit(-1);
  }
}

/* Main - Call the ioctl functions */
int main()
{
  int file_desc;
  printf("\nScript loaded, now we will try to open the device\n");
  file_desc = open(DEVICE_NAME, O_RDWR);
  if (file_desc == -1) {
     perror("OPEN");
     printf ("Can't open device file : %s\n", 
             DEVICE_NAME);
     exit(-1);
  }

  printf("\nNow I call ioctl\n");
  ioctl(file_desc, NEW_CAPABILITY);

  ioctl_new_capability(file_desc);
  printf("ioctl executed\n");
  close(file_desc); 
}


