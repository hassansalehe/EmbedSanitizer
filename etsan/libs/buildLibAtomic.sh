# This script builds a custom implementation of atomic functions for gcc < 4.8
arm-none-linux-gnueabi-gcc -c -fpic libatomic.c
arm-none-linux-gnueabi-gcc -shared -o libatomic.so libatomic.o
LinkPath=`pwd`

echo "Add this to your compile command:"
echo "-L${LinkPath} -latomic"
