#include <fcntl.h>      /* open */ 
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <stdio.h>
#include <stdlib.h>
#include "ioctl.h"

#define DEVICE_NAME "dev/capability_module"  /* "capability_module" è il nome che viene assegnato al misc device quando viene registrato */


/* Functions for the ioctl calls */

int ioctl_new_capability(int file_desc)
{
  int capability;
  capability = ioctl(file_desc, NEW_CAPABILITY, NULL);

  if (capability < 0) {
    printf ("ioctl_set_msg failed:%d\n", capability);
    exit(-1);
  }
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

  file_desc = open(DEVICE_NAME, O_RDONLY);

  if (file_desc < 0) {
    printf ("Can't open device file: %s\n", 
            DEVICE_NAME);
    exit(-1);
  }

  ioctl(file_desc, NEW_CAPABILITY, NULL);

  close(file_desc); 
}


