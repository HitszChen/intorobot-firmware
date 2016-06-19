## other flags
CFLAGS += -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16
CFLAGS += -DSTM32F411xE
CFLAGS += -DUSE_HAL_DRIVER

# FLAGS For Neutron
CFLAGS += -I neutron/inc/inc_board/
CFLAGS += -I neutron/inc/inc_mcu/

LDFLAGS += -L neutron/lib/ -Wl,--whole-archive -lboard -lplatform -lPDMFilter_CM4_GCC -Wl,--no-whole-archive
LDFLAGS += -T neutron/linker/linker_stm32f411xe_boot.ld

STARTUP_OBJFILE += neutron/startup/startup_stm32f411xe.o



