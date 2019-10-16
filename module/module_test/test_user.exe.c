#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include "../ioctl.h"

int main()
{
	// bisogna definire fd !!!!!!!!!!!!!!!! DObbiamo definire una my_open da chiamare con open() ? sì dobbiamo
	// qua però c'è scritto che un miscdevice non alloca un major number, è un problema? https://stackoverflow.com/questions/18456155/what-is-the-difference-between-misc-drivers-and-char-drivers
	ret = ioctl(fd, NEW_CAPABILITY);
        if (ret == -1) {
            perror("ioctl");
            return EXIT_FAILURE;
        }
	return EXIT_SUCCESS;
}
