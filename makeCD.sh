#! /bin/sh
#automating script for making CD games


DIR=$PWD
PROJECT_NAME="Romdisk_test"

# go to build directory
# make clean
make

#elf transform??
sh-elf-objcopy -R .stack -O binary main.elf output.bin

#scraming process
$KOS_BASE/utils/scramble/scramble output.bin 1ST_READ.bin

#creating iso -> -o outputname.cdi / build files path
mkisofs -C 0,11702 -V DC_GAME -G IP.BIN -r -J -l -m '*.o' -o ../$PROJECT_NAME.iso $DIR

#transform iso into a CDI
$KOS_BASE/utils/cdi4cd/cdi4dc ../$PROJECT_NAME.iso ../$PROJECT_NAME.cdi -d > cdi4dc.log

#YOU MIGHT WANT TO EDIT THE IP ADRESS HERE !!!!
sudo arp -s 192.168.0.99 00:d0:f1:03:14:02

#start the game
sudo ../dc-tool-ip -t dreamcast -c ./ -x main.elf
#lxdream ../$PROJECT_NAME.iso
#reicast ../$PROJECT_NAME.cdi
#redream ../$PROJECT_NAME.cdi
