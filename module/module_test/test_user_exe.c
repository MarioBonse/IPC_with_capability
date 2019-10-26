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
  if (capability == 0) {
    printf ("ioctl_new failed:%d\n", capability);
		return 0;
  }
  return capability;
}

int ioctl_read_capability(int file_desc, int capability, char*buff, int len)
{
  int ret;
  struct ioctl_message *message = malloc(sizeof(struct ioctl_message));
  message->capability = capability;
  message->buff = buff;
  message->len = len;
  ret = ioctl(file_desc, READ_CAPABILITY, message);
  // copy messagge.buff in messagge
  if (ret < 0) {
    printf ("ioctl_read_msg failed:%d\n", ret);
    return (-1);
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
    printf ("ioctl_write_msg failed:%d\n", ret);
    return(-1);
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
    return(-1);
  }
  return ret;
}

/* Main - Call the ioctl functions */
int main()
{
  int len;
  int capability;
  char text[]="Message_example";
  int message_len = sizeof(text);
  int file_desc;
  char *message = malloc(sizeof(text)*sizeof(char));
  char *message_2 = malloc(sizeof(text)*sizeof(char));

  printf("\nScript loaded, now we will try to open the device\n");
  file_desc = open(DEVICE_NAME, O_RDWR);
  if (file_desc == -1) {
     perror("OPEN");
     printf ("Can't open device file : %s\n", 
             DEVICE_NAME);
     exit(-1);
  }
	

	printf("\nNow we will try to create a new capability\n");
  capability = ioctl_new_capability(file_desc);
	printf("\nNow we will try to write on the capability\n");
  len = ioctl_write_capability(file_desc, capability, text, message_len);

  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  printf("We wrote %d character\n", len);
  printf("Original text is: %s\n", text);
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

	printf("\nNow we will try to write again on the same capability, without having read from it first (this should fail)\n");
	ioctl_write_capability(file_desc, capability, text, message_len);
	
	printf("\nNow we will read the capability\n");
  len = ioctl_read_capability(file_desc, capability, message, message_len);

  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  printf("Received %d byte\n", len);
  printf("Received text is: %s\n", message); 
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

	printf("\nNow we will try to read again from the same capability\n");
	len = ioctl_read_capability(file_desc, capability, message_2, message_len);

	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  printf("Received %d byte\n", len);
  printf("Received text is: %s\n", message_2); 
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");


	printf("\nNow we will create a different capability, write on it and then read from it\n");
	int len_2;
  int capability_2;
	char text_2[]="Second_message";
  int message_len_2 = sizeof(text_2);

	capability_2 = ioctl_new_capability(file_desc);

	len_2 = ioctl_write_capability(file_desc, capability_2, text_2, message_len_2);
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  printf("We wrote %d character\n", len_2);
  printf("Original text is: %s\n", text_2);
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

	len_2 = ioctl_read_capability(file_desc, capability_2, message, message_len_2);
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
  printf("Received %d byte\n", len_2);
  printf("Received text is: %s\n", message); 
  printf("!!!!!!!!!!!!!!!!!!!!!!!!!!\n");


  printf("\nNow we delete the capabilities\n");
  ioctl_del_capability(file_desc, capability);
  ioctl_del_capability(file_desc, capability_2);
  printf("As the capailities has been deleted now we can't write on it anymore\n");
  len = ioctl_write_capability(file_desc, capability, text, 5);

  close(file_desc); 
}


