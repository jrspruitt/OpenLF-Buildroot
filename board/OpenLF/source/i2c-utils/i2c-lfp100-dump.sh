#!/bin/sh
# Read LFP100 registers
CHANNEL="/dev/i2c-0"

# i2c interface changed after $BYTES.6.31
if [ `uname -r` == "2.6.31-leapfrog" ]; then
        ADDR="0xCD"
        BYTES=2
else
        ADDR="0x66"
        BYTES=1
fi

#
# '*PW' in register name listing means this is a password protected register
#
echo -n "CHIPID                      (0x00): "; i2c-ctl $CHANNEL r $ADDR 0x00 $BYTES
echo -n "CONTROL                     (0x01): "; i2c-ctl $CHANNEL r $ADDR 0x01 $BYTES
echo -n "STATUS 1                    (0x02): "; i2c-ctl $CHANNEL r $ADDR 0x02 $BYTES
echo -n "STATUS 2                    (0x03): "; i2c-ctl $CHANNEL r $ADDR 0x03 $BYTES
echo -n "INT 1                       (0x04): "; i2c-ctl $CHANNEL r $ADDR 0x04 $BYTES
echo -n "INT 2                       (0x05): "; i2c-ctl $CHANNEL r $ADDR 0x05 $BYTES
echo -n "INT 3                       (0x06): "; i2c-ctl $CHANNEL r $ADDR 0x06 $BYTES
echo -n "MASK 1                      (0x07): "; i2c-ctl $CHANNEL r $ADDR 0x07 $BYTES
echo -n "MASK 2                      (0x08): "; i2c-ctl $CHANNEL r $ADDR 0x08 $BYTES
echo -n "MASK 3                      (0x09): "; i2c-ctl $CHANNEL r $ADDR 0x09 $BYTES
echo -n "WLED                        (0x0A): "; i2c-ctl $CHANNEL r $ADDR 0x0a $BYTES
echo -n "PPATH                       (0x0B): "; i2c-ctl $CHANNEL r $ADDR 0x0b $BYTES
echo -n "IO                          (0x0C): "; i2c-ctl $CHANNEL r $ADDR 0x0c $BYTES
echo -n "PASSWORD                    (0x0D): "; i2c-ctl $CHANNEL r $ADDR 0x0d $BYTES
echo -n "P_ENABLE                    (0x0E): "; i2c-ctl $CHANNEL r $ADDR 0x0e $BYTES
echo -n "DCDC1 *PW                   (0x0F): "; i2c-ctl $CHANNEL r $ADDR 0x0f $BYTES
echo -n "DCDC2 *PW                   (0x10): "; i2c-ctl $CHANNEL r $ADDR 0x10 $BYTES
echo -n "SLEW *PW                    (0x11): "; i2c-ctl $CHANNEL r $ADDR 0x11 $BYTES
echo -n "LDO1 *PW                    (0x12): "; i2c-ctl $CHANNEL r $ADDR 0x12 $BYTES
echo -n "LDO2 *PW                    (0x13): "; i2c-ctl $CHANNEL r $ADDR 0x13 $BYTES
echo -n "LDO3 *PW                    (0x14): "; i2c-ctl $CHANNEL r $ADDR 0x14 $BYTES
echo -n "PG *PW                      (0x15): "; i2c-ctl $CHANNEL r $ADDR 0x15 $BYTES
echo -n "UVLO *PW                    (0x16): "; i2c-ctl $CHANNEL r $ADDR 0x16 $BYTES
echo -n "SEQ1 *PW                    (0x17): "; i2c-ctl $CHANNEL r $ADDR 0x17 $BYTES
echo -n "SEQ2 *PW                    (0x18): "; i2c-ctl $CHANNEL r $ADDR 0x18 $BYTES
echo -n "SEQ3 *PW                    (0x19): "; i2c-ctl $CHANNEL r $ADDR 0x19 $BYTES
echo -n "SEQ4 *PW                    (0x1A): "; i2c-ctl $CHANNEL r $ADDR 0x1a $BYTES
echo -n "SEQ5 *PW                    (0x1B): "; i2c-ctl $CHANNEL r $ADDR 0x1b $BYTES
echo -n "FORMAT *PW                  (0x1C): "; i2c-ctl $CHANNEL r $ADDR 0x1c $BYTES
echo -n "FILTER                      (0x1D): "; i2c-ctl $CHANNEL r $ADDR 0x1d $BYTES
echo -n "A_APOP *PW                  (0x1E): "; i2c-ctl $CHANNEL r $ADDR 0x1e $BYTES
echo -n "A_CONTROL                   (0x1F): "; i2c-ctl $CHANNEL r $ADDR 0x1f $BYTES
echo -n "GAINSL                      (0x20): "; i2c-ctl $CHANNEL r $ADDR 0x20 $BYTES
echo -n "GAINADJ *PW                 (0x21): "; i2c-ctl $CHANNEL r $ADDR 0x21 $BYTES
echo -n "MGAIN                       (0x22): "; i2c-ctl $CHANNEL r $ADDR 0x22 $BYTES
echo -n "VOLUME                      (0x23): "; i2c-ctl $CHANNEL r $ADDR 0x23 $BYTES
echo -n "VLIMIT *PW                  (0x24): "; i2c-ctl $CHANNEL r $ADDR 0x24 $BYTES
echo -n "MICGAIN                     (0x25): "; i2c-ctl $CHANNEL r $ADDR 0x25 $BYTES
echo -n "TEST31H                     (0x31): "; i2c-ctl $CHANNEL r $ADDR 0x31 $BYTES
echo -n "TEST40H                     (0x40): "; i2c-ctl $CHANNEL r $ADDR 0x40 $BYTES
echo -n "TEST41H                     (0x41): "; i2c-ctl $CHANNEL r $ADDR 0x41 $BYTES
echo -n "TEST42H                     (0x42): "; i2c-ctl $CHANNEL r $ADDR 0x42 $BYTES
echo -n "TEST43H                     (0x43): "; i2c-ctl $CHANNEL r $ADDR 0x43 $BYTES
echo -n "TEST4BH                     (0x4B): "; i2c-ctl $CHANNEL r $ADDR 0x4B $BYTES
echo -n "TEST4DH                     (0x4D): "; i2c-ctl $CHANNEL r $ADDR 0x4D $BYTES
