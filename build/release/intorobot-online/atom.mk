## other flags

CFLAGS += -mcpu=cortex-m3
CFLAGS += -DSTM32F10X_MD
CFLAGS += -DUSE_STDPERIPH_DRIVER

LDFLAGS += -nostartfiles

# FLAGS For Atom

CFLAGS += -I atom/inc/inc_board/
CFLAGS += -I atom/inc/inc_mcu/
CFLAGS += -I atom/inc/inc_board/ros_lib/
#CPPFLAGS = 

LDFLAGS += -L atom/lib/ -lboard
LDFLAGS += -L atom/lib/ -lplatform
LDFLAGS += -T atom/linker/linker_stm32f10x_md.ld

STARTUP_OBJFILE += atom/startup/startup_stm32f10x_md.o


