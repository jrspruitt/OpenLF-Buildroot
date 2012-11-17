LIBS=-lrt -lasound

all: lfd lfd-ctrl

MORE_CFLAGS=-std=gnu99 -Wno-variadic-macros -Wall -pedantic

lfd: lfd.c sound.c daemon.c socket.c i2c.c backlight.c time.c nor.c battery.c
	$(CC) $(CFLAGS) $(MORE_CFLAGS) -o $@ $^ $(LIBS)

lfd-ctrl: lfd-ctrl.c socket.c
	$(CC) $(CFLAGS) $(MORE_CLFAGS) -o $@ $^

clean:
	rm -f *.o lfd lfd-ctrl

install: lfd lfd-ctrl
	cp lfd \
		lfd-ctrl $(ROOTFS_PATH)/usr/bin
