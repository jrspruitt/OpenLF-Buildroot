############################################################################
#
# lfd
#
############################################################################

LFD_VERSION = 0.1
LFD_SOURCE = lfd_v$(LFD_VERSION).tar.gz
LFD_SITE = $(TOPDIR)/board/OpenLF/source
LFD_SITE_METHOD = file
LFD_INSTALL_STAGING = NO
LFD_DEPENDENCIES = -lrt -lasound

define LFD_BUILD_CMDS
	$(MAKE) CC="$(TARGET_CC)" -C $(@D)
endef

define LFD_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/lfd $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/lfd-ctrl $(TARGET_DIR)/usr/bin
endef



$(eval $(generic-package))
