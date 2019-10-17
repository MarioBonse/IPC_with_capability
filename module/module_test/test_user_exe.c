#include <fcntl.h>      /* open */ 
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include "ioctl.h"

#define DEVICE_FILE_NAME "capability_module"  /* oppure ci va "test", che è il nome del miscdevice? */


/* Functions for the ioctl calls */

ioctl_new_capability(int file_desc)
{
  int capability;
  capability = ioctl(file_desc, NEW_CAPABILITY, NULL);

  if (capability < 0) {
    printf ("ioctl_set_msg failed:%d\n", capability);
    exit(-1);
  }
}

ioctl_send_capability(int file_desc, int capability, char*buff)
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
main()
{
  int file_desc;

  file_desc = open(DEVICE_FILE_NAME, 0);

  if (file_desc < 0) {
    printf ("Can't open device file: %s\n", 
            DEVICE_FILE_NAME);
    exit(-1);
  }

  ioctl_new_capability(file_desc)

  close(file_desc); 
}


