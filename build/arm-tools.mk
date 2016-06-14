#
#  定义ARM 编译工具


# 定义编译工具前缀
GCC_PREFIX ?= arm-none-eabi-

include $(COMMON_BUILD)/common-tools.mk

#
# 默认参数
#

# C 编译参数
ifeq ($(PLATFORM_ID),0) # atom
CFLAGS += -mcpu=cortex-m3
else
CFLAGS += -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16
endif

# 如果程序没有按预期工作   尝试去掉-fdata-sections
CFLAGS += -g3 -gdwarf-2 -Os -mthumb -fno-strict-aliasing -Wfatal-errors -w -fno-common -ffunction-sections -fdata-sections -Wno-switch -Wno-error=deprecated-declarations -fmessage-length=0

# C++ 编译参数
#CPPFLAGS += -fno-exceptions -fno-rtti -std=gnu++11 -fcheck-new
CPPFLAGS += $(CFLAGS) -fno-exceptions -fno-rtti -std=gnu++11 -fcheck-new

ifeq ($(PLATFORM_ID),0)
ASFLAGS += -mcpu=cortex-m3
LDFLAGS += -nostartfiles
else
ASFLAGS += -mcpu=cortex-m4
endif

ASFLAGS += -g3 -gdwarf-2 -mthumb -x assembler-with-cpp -fmessage-length=0

LDFLAGS += -mlittle-endian -Xlinker --gc-sections --specs=nano.specs --specs=nosys.specs -lc -lnosys
LDFLAGS += -Wl,--start-group -lgcc -lc -Wl,--end-group

ifeq ($(COMPILE_LTO),y)
CFLAGS += -flto
LDFLAGS += -flto -Os -fuse-linker-plugin
endif

# 检查编译器版本是否符合要求
# Check if the compiler version is the minimum required
arm_gcc_version:=$(shell $(CC) --version | head -n 1)
arm_gcc_version:=$(strip $(subst arm-none-eabi-gcc (GNU Tools for ARM Embedded Processors),,$(arm_gcc_version)))
expected_version:=4.8.4
quote="
lt=\<
dollar=$$
#$(info result $(shell test $(quote)$(arm_gcc_version)$(quote) $(lt) $(quote)$(expected_version)$(quote);echo $$?))
ifeq ($(shell test $(quote)$(arm_gcc_version)$(quote) $(lt) $(quote)$(expected_version)$(quote); echo $$?),0)
     $(error "ARM gcc version $(expected_version) or later required, but found $(arm_gcc_version)")
endif


ifeq ($(shell test $(quote)$(arm_gcc_version)$(quote) $(lt) $(quote)4.9.0$(quote); echo $$?),0)
     NANO_SUFFIX=_s
endif
