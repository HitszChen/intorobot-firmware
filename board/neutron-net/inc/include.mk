INCLUDE_DIRS += $(BOARD_TYPE_PATH)/inc

ifneq (,$(findstring board,$(MAKE_DEPENDENCIES)))

LDFLAGS += -T$(COMMON_BUILD)/linker/esp8266/linker_esp8266-net.ld

#LDFLAGS += -g -w -Os -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,register_chipv6_phy
LDFLAGS += -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static
LIBS += c gcc hal phy pp net80211 lwip wpa crypto main json ssl pwm wps smartconfig airkiss at
#LIBS_EXT += -Wl,--start-group -lc -lgcc -lhal -lphy -lpp -lnet80211 -llwip -lwpa -lcrypto -lmain -ljson -lssl -lpwm -lwps -lsmartconfig -lairkiss -lat -Wl,--end-group

endif
