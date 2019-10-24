KERNEL_DIR ?= /home/mario/git/KernelProgrammingProject/kernel/linux-5.2.7
PWD := $(shell pwd)

obj-m = capability_module.o

all:
	make -C $(KERNEL_DIR) M=`pwd` modules

clean:
	make -C $(KERNEL_DIR) M=`pwd` clean
