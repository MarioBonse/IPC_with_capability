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

int ioctl_del_capability(int file_desc, int capability)
{
  int ret;
  struct ioctl_message *messagge = malloc(sizeof(struct ioctl_message));
  messagge->capability = capability;
  messagge->buff = NULL;
  messagge->len = 0;
  ret = ioctl(file_desc, DEL_CAPABILITY, messagge);
  // copy messagge.buff in messagge
  if (ret == 0) {
    printf ("ioctl_delete failed:%d\n", ret);
    exit(-1);
  }
  return ret;
}

/* Main - Call the ioctl functions */
int main()
{
  char *message = malloc(5*sizeof(char));
  int len;
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
  len = ioctl_write_capability(file_desc, capability, text, 5);

  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  printf("We wrote %d character\n", len);
  printf("Original text is: %s\n", text);
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

  len = ioctl_read_capability(file_desc, capability, message, 5);

  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  printf("recived %d byte\n", len);
  printf("Recived text is: %s\n", message); 
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  printf("Now I del the capability");

  ioctl_del_capability(file_desc, capability);
  printf("Now I can't write anymore the capability\n");
  len = ioctl_write_capability(file_desc, capability, text, 5);

  close(file_desc); 
}


