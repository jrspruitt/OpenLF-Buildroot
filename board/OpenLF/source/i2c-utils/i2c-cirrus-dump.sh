#!/bin/sh
# Read Cirrus Logic registers
CHANNEL="/dev/i2c-0"
echo -n "ID                          (0x01): "; i2c-ctl $CHANNEL r 0x95 0x01 2
echo -n "Power Ctl 1                 (0x02): "; i2c-ctl $CHANNEL r 0x95 0x02 2
echo -n "Power Ctl 2                 (0x03): "; i2c-ctl $CHANNEL r 0x95 0x03 2
echo -n "Power Ctl 3                 (0x04): "; i2c-ctl $CHANNEL r 0x95 0x04 2
echo -n "Clocking Ctl                (0x05): "; i2c-ctl $CHANNEL r 0x95 0x05 2
echo -n "Interface Ctl 1             (0x06): "; i2c-ctl $CHANNEL r 0x95 0x06 2
echo -n "Interface Ctl 2             (0x07): "; i2c-ctl $CHANNEL r 0x95 0x07 2
echo -n "Input A Select              (0x08): "; i2c-ctl $CHANNEL r 0x95 0x08 2
echo -n "Input B Select              (0x09): "; i2c-ctl $CHANNEL r 0x95 0x09 2
echo -n "Analog, HPF Ctl             (0x0A): "; i2c-ctl $CHANNEL r 0x95 0x0a 2
echo -n "ADC HPF Corner Freq         (0x0B): "; i2c-ctl $CHANNEL r 0x95 0x0b 2
echo -n "Misc ADC Ctl                (0x0C): "; i2c-ctl $CHANNEL r 0x95 0x0c 2
echo -n "Playback Ctl 1              (0x0D): "; i2c-ctl $CHANNEL r 0x95 0x0d 2
echo -n "Misc Ctl                    (0x0E): "; i2c-ctl $CHANNEL r 0x95 0x0e 2
echo -n "Playback Ctl 2              (0x0F): "; i2c-ctl $CHANNEL r 0x95 0x0f 2
echo -n "MICA Amp Ctl                (0x10): "; i2c-ctl $CHANNEL r 0x95 0x10 2
echo -n "MICB Amp Ctl                (0x11): "; i2c-ctl $CHANNEL r 0x95 0x11 2
echo -n "PGAA Vol, Misc              (0x12): "; i2c-ctl $CHANNEL r 0x95 0x12 2
echo -n "PGAB Vol, Misc              (0x13): "; i2c-ctl $CHANNEL r 0x95 0x13 2
echo -n "Passthrou A Vol             (0x14): "; i2c-ctl $CHANNEL r 0x95 0x14 2
echo -n "Passthrou B Vol             (0x15): "; i2c-ctl $CHANNEL r 0x95 0x15 2
echo -n "ADCA Vol                    (0x16): "; i2c-ctl $CHANNEL r 0x95 0x16 2
echo -n "ADCB Vol                    (0x17): "; i2c-ctl $CHANNEL r 0x95 0x17 2
echo -n "ADCMIXA Vol                 (0x18): "; i2c-ctl $CHANNEL r 0x95 0x18 2
echo -n "ADCMIXB Vol                 (0x19): "; i2c-ctl $CHANNEL r 0x95 0x19 2
echo -n "PCMMIXA Vol                 (0x1A): "; i2c-ctl $CHANNEL r 0x95 0x1a 2
echo -n "PCMMIXB Vol                 (0x1B): "; i2c-ctl $CHANNEL r 0x95 0x1b 2
echo -n "BEEP Freq, On Time          (0x1C): "; i2c-ctl $CHANNEL r 0x95 0x1c 2
echo -n "BEEP Vol, Off Time          (0x1D): "; i2c-ctl $CHANNEL r 0x95 0x1d 2
echo -n "BEEP, Tone Cfg              (0x1E): "; i2c-ctl $CHANNEL r 0x95 0x1e 2
echo -n "Tone Ctl                    (0x1F): "; i2c-ctl $CHANNEL r 0x95 0x1f 2
echo -n "Master A Vol                (0x20): "; i2c-ctl $CHANNEL r 0x95 0x20 2
echo -n "Master B Vol                (0x21): "; i2c-ctl $CHANNEL r 0x95 0x21 2
echo -n "Headphone A Volume          (0x22): "; i2c-ctl $CHANNEL r 0x95 0x22 2
echo -n "Headphone B Volume          (0x23): "; i2c-ctl $CHANNEL r 0x95 0x23 2
echo -n "Speaker A Volume            (0x24): "; i2c-ctl $CHANNEL r 0x95 0x24 2
echo -n "Speaker B Volume            (0x25): "; i2c-ctl $CHANNEL r 0x95 0x25 2
echo -n "Channel Mixer & Swap        (0x26): "; i2c-ctl $CHANNEL r 0x95 0x26 2
echo -n "Limit Ctl 1, Thresholds     (0x27): "; i2c-ctl $CHANNEL r 0x95 0x27 2
echo -n "Limit Ctl 2, Thresholds     (0x28): "; i2c-ctl $CHANNEL r 0x95 0x28 2
echo -n "Limiter Attack Rate         (0x29): "; i2c-ctl $CHANNEL r 0x95 0x29 2
echo -n "ALC Ctl 1, Attack Rate      (0x2A): "; i2c-ctl $CHANNEL r 0x95 0x2a 2
echo -n "ALC Release Rate            (0x2B): "; i2c-ctl $CHANNEL r 0x95 0x2b 2
echo -n "ALC Thresholds              (0x2C): "; i2c-ctl $CHANNEL r 0x95 0x2c 2
echo -n "Noise Gate Ctl              (0x2D): "; i2c-ctl $CHANNEL r 0x95 0x2d 2
echo -n "Overflow and Clock Status   (0x2E): "; i2c-ctl $CHANNEL r 0x95 0x2e 2
echo -n "Battery Compensation        (0x2F): "; i2c-ctl $CHANNEL r 0x95 0x2f 2
echo -n "VP Battery Level            (0x30): "; i2c-ctl $CHANNEL r 0x95 0x30 2
echo -n "Speaker Status              (0x31): "; i2c-ctl $CHANNEL r 0x95 0x31 2
echo -n "Temperature Monitor Control (0x32): "; i2c-ctl $CHANNEL r 0x95 0x32 2
echo -n "Thermal Foldback            (0x33): "; i2c-ctl $CHANNEL r 0x95 0x33 2
echo -n "Charge Pump Frequency       (0x34): "; i2c-ctl $CHANNEL r 0x95 0x34 2
