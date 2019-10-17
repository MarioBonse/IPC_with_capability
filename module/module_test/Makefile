KERNEL_DIR ?= /lib/modules/`uname -r`/build

obj-m = capability_module.o

all:
	make -C $(KERNEL_DIR) M=`pwd` modules
	cc test_user_exe.c -o test_user_exe

clean:
	make -C $(KERNEL_DIR) M=`pwd` clean

