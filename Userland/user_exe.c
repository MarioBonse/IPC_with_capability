#include <fcntl.h>      /* open */ 
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */


#DEFINE DEVICE_FILE_NAME "test_mod"  /* oppure ci va "test", che è il nome del miscdevice? */


/* Functions for the ioctl calls */

ioctl_new_capability(int file_desc)
{
  ioctl(file_desc, NEW_CAPABILITY);

  if (ret_val < 0) {
    printf ("ioctl_set_msg failed:%d\n", ret_val);
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

  ioctl_new_capability(file_desc);

  close(file_desc); 
}


