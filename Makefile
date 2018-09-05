DIR = $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
TARGET = main.elf
OBJS = main.o

KOS_CFLAGS += -std=c99 -I$(KOS_PORTS)/include/lua
KOS_CFLAGS += -I$(DIR)scenes/ -I$(DIR)lib/
#KOS_ROMDISK_DIR = romdisk

all: rm-elf $(TARGET)

include $(KOS_BASE)/Makefile.rules

clean:
	@$(RM) $(TARGET) $(OBJS)romdisk.* *.cdi *.iso *.img *.gz

rm-elf:
	@$(RM) $(TARGET) romdisk.*

$(TARGET): $(OBJS) romdisk.o
	$(KOS_CC) $(KOS_CFLAGS) $(KOS_LDFLAGS) -o $(TARGET) $(KOS_START) \
		$(OBJS) romdisk.o $(OBJEXTRA) -llua -lm -lz  $(KOS_LIBS)

#make a bunch of romdisk here
romdisk.img:
	$(KOS_GENROMFS) -f romdisk.img -d romdisk
	$(KOS_GENROMFS) -f asset/rd_test.img -d asset/rd_test

	gzip -f -9 asset/rd_test.img

romdisk.o: romdisk.img
	$(KOS_BASE)/utils/bin2o/bin2o romdisk.img romdisk romdisk.o

run: $(TARGET)
	$(KOS_LOADER) $(TARGET)

dist:
	rm -f $(OBJS) romdisk.o romdisk.img
	$(KOS_STRIP) $(TARGET)
