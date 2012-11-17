CFLAGS = -O
all: i2c-ctrl

i2c-ctl: i2c-ctl.c i2c.h i2c-dev.h
	$(WARNING WARNING WARNING WARNING)
	$(CC) $(CFLAGS) -o i2c-ctl i2c-ctl.c

clean:
	rm -f i2c-ctl

install: i2c-ctl i2c-cirrus-dump.sh i2c-cirrus-setup.sh i2c-lfp100-dump.sh
	cp i2c-ctl \
	   i2c-cirrus-dump.sh \
	   i2c-cirrus-setup.sh \
	   i2c-lfp100-dump.sh $(ROOTFS_PATH)/usr/bin
