#!/bin/sh
# make
CURR_DIRECTORY=$(pwd)
MODULE_REL_PATH="module/module_test"
KERNELVERSION=$(ls "$CURR_DIRECTORY/kernel")
KERNELDIR="$CURR_DIRECTORY/kernel/$KERNELVERSION"

# First move in the module directory and compile it
cd $MODULE_REL_PATH
make -C $KERNELDIR M=$(pwd)
# eventually create the directory that contains the module
cd $CURR_DIRECTORY
mkdir Test
cp $MODULE_REL_PATH/*.ko Test/
find Test | cpio -o -H newc | gzip > mod.gz
cat tinyfs.gz mod.gz > test.gz
qemu-system-x86_64 -kernel "$KERNELDIR/arch/x86_64/boot/bzImage" -initrd test.gz
