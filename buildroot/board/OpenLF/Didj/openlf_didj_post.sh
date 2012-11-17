#!/bin/sh
TARGET=output/target
BOARD_DIR=board/OpenLF/Didj
RFS_FILES=$BOARD_DIR/files
BULK_MNT=$TARGET/mnt/data
SD_MNT=$TARGET/mnt/sd

# Add board files to rfs
if [ ! -e $RFS_FILES ]; then
    echo 'RFS Files directory missing'
    exit 1
else
    cp -rf $RFS_FILES/* $TARGET/
fi

# delete unneeded rfs files
for dfile in $(cat $BOARD_DIR/file_delete.list);
do
    if [ -e $TARGET/$dfile ]; then
        rm -r $TARGET/$dfile
    fi
done;

# create mount points
if [ ! -e $BULK_MNT ]; then
	mkdir $BULK_MNT
fi

if [ ! -e $SD_MNT ]; then
	mkdir $SD_MNT
fi

# Fix linuxrc/init name
if [ -e $TARGET/linuxrc ]; then
    mv $TARGET/linuxrc $TARGET/init
fi


# Fix ssh
if [ ! -e $TARGET/etc/ssh/ ]; then
    mkdir $TARGET/etc/ssh
fi

if [ -e $TARGET/etc/sshd_config ]; then
    sed -i "s/#PermitEmptyPasswords no/PermitEmptyPasswords yes/" $TARGET/etc/sshd_config
    sed -i "s/#HostKey \/etc\//HostKey \/etc\/ssh\//" $TARGET/etc/sshd_config
    mv $TARGET/etc/ssh*_config $TARGET/etc/ssh/
fi

if [ -e $TARGET/etc/init.d/S50sshd ]; then
    sed -i "s/\/etc\/ssh_/\/etc\/ssh\/ssh_/" $TARGET/etc/init.d/S50sshd
    if [ $(grep -c "/usr/sbin/sshd -f /etc/ssh/sshd_config" $TARGET/etc/init.d/S50sshd) -eq 0 ]; then
        sed -i "s/\/usr\/sbin\/sshd/\/usr\/sbin\/sshd -f \/etc\/ssh\/sshd_config/" $TARGET/etc/init.d/S50sshd
    fi
fi

# inittab fix

if [ -e $TARGET/etc/inittab ]; then
    sed -i "s/null::sysinit:\/bin\/hostname/::sysinit:\/bin\/hostname/" $TARGET/etc/inittab
    if [ $(grep -c "tty0::respawn:-/bin/sh" $TARGET/etc/inittab) -eq 0 ]; then
        echo "
# console on screen
tty0::respawn:-/bin/sh" >> $TARGET/etc/inittab
    fi
fi
