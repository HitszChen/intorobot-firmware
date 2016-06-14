#
# 定义xtensa编译工具相关参数
#

# 定义编译器和工具的前缀
GCC_PREFIX ?= xtensa-lx106-elf-

include $(COMMON_BUILD)/common-tools.mk


# C 编译参数
CFLAGS += -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -MMD -std=gnu99 -ffunction-sections -fdata-sections -DICACHE_FLASH

#CFLAGS += -Os -g -Wpointer-arith -Wundef -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -ffunction-sections -fdata-sections  -DICACHE_FLASH

# C++ 编译参数
CPPFLAGS += -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -MMD -ffunction-sections -fdata-sections

ASFLAGS += -c -g -x assembler-with-cpp -MMD

# Board definitions
FLASH_SIZE ?= 4M
FLASH_MODE ?= qio
FLASH_SPEED ?= 40

# Upload parameters
UPLOAD_SPEED ?= 230400
UPLOAD_PORT ?= /dev/cu.usbmodem1411
UPLOAD_VERB ?= -v
UPLOAD_RESET ?= ck

#

