############################################################################
#
# lfd
#
############################################################################

LFD_VERSION = master
LFD_SITE = https://github.com/jrspruitt/OpenLF-lfd.git 
LFD_SITE_METHOD = git
LFD_INSTALL_STAGING = NO
LFD_DEPENDENCIES = -lrt alsa-lib

define LFD_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D)
endef

define LFD_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/lfd $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/lfd-ctrl $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))
