############################################################################
#
# i2c-utils
#
############################################################################

I2C_UTILS_VERSION = 0.1
I2C_UTILS_SOURCE = i2c-utils_v$(I2C_UTILS_VERSION).tar.gz
I2C_UTILS_SITE = $(TOPDIR)/board/OpenLF/source
I2C_UTILS_SITE_METHOD = file
I2C_UTILS_INSTALL_STAGING = NO
I2C_UTILS_DEPENDENCIES = -lrt -lasound

define I2C_UTILS_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D)
endef

define I2C_UTILS_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/i2c-ctl $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/i2c-cirrus-dump.sh $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/i2c-cirrus-setup.sh $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/i2c-lfp100-dump.sh $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))
