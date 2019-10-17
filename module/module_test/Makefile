KERNEL_DIR ?= /lib/modules/`uname -r`/build

obj-m = test_mod.o

all:
	make -C $(KERNEL_DIR) M=`pwd` modules
	cc test_client.c -o test_client

clean:
	make -C $(KERNEL_DIR) M=`pwd` clean

