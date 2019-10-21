#!/bin/sh
# make
CURR_DIRECTORY=$(pwd)
MODULE_REL_PATH="module/module_test"
KERNELVERSION=$(ls "$CURR_DIRECTORY/kernel")
KERNELDIR="$CURR_DIRECTORY/kernel/$KERNELVERSION"

# First move in the module directory and compile it
cd $MODULE_REL_PATH
make clean
make -C $KERNELDIR M=$(pwd)
rm test_user_exe
cc test_user_exe.c -o test_user_exe
# eventually create the directory that contains the module
cd $CURR_DIRECTORY
rm -r Test
mkdir Test
cp $MODULE_REL_PATH/*.ko Test/
cp $MODULE_REL_PATH/test_user_exe Test/
find Test | cpio -o -H newc | gzip > mod.gz
cat tinyfs.gz mod.gz > test.gz
qemu-system-x86_64 -kernel "$KERNELDIR/arch/x86_64/boot/bzImage" -initrd test.gz
