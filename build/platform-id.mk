ifeq ($(included_productid_mk),)
included_productid_mk := 1

# defines
# PLATFORM_NAME - a unique name for the platform, can be used to organise sources
#                 by platform
# PLATFORM_MCU  - an identifier for the MCU family
# PLATFORM_NET  - the network subsystem
# STM32_DEVICE  - the specific device being targeted for STM32 platform builds
# ARCH		- architecture (ARM/GCC)
# PRODUCT_DESC  - text description of the product ID
# PLATFORM_DYNALIB_MODULES - if the device supports a modular build, the name
#		- of the subdirectory containing

# Default USB Device Vendor ID for Intorobot Products
USBD_VID_INTOROBOT=0x1D50
# Default USB Device Product ID for DFU Class
USBD_PID_DFU=0x607F
# Default USB Device Product ID for CDC Class
USBD_PID_CDC=0x607D

ifneq (,$(PLATFORM))
ifeq ("$(PLATFORM)","atom")
PLATFORM_ID = 0
endif

ifeq ("$(PLATFORM)","neutron")
PLATFORM_ID = 1
endif

ifeq ("$(PLATFORM)","gcc")
PLATFORM_ID = 2
endif

ifeq ("$(PLATFORM)","neutron-net")
PLATFORM_ID = 3
endif

ifeq ("$(PLATFORM)","electron")
PLATFORM_ID = 4
endif

ifeq ("$(PLATFORM)","newhal")
PLATFORM_ID=60000
endif

ifeq (,$(PLATFORM_ID))
$(error "Unknown platform: $(PLATFORM))
endif
endif

ifndef PLATFORM_ID
PLATFORM_ID=0
endif

# Determine which is the target device

ARCH=arm

ifeq ("$(PLATFORM_ID)","0") #atom
STM32_DEVICE=STM32F10X_MD
PLATFORM=atom
PLATFORM_NAME=atom
PLATFORM_MCU=STM32F1xx
PRODUCT_DESC=IntoRobot Atom, 128k flash, 20k sram
USBD_VID_INTOROBOT=0x1D50
USBD_PID_DFU=0x607F
USBD_PID_CDC=0x607D
DEFAULT_PRODUCT_ID=0
endif

ifeq ("$(PLATFORM_ID)","1") #neutron
PLATFORM=neutron
STM32_DEVICE=STM32F411xE
PLATFORM_NAME=neutron
PLATFORM_MCU=STM32F4xx
PRODUCT_DESC=IntoRobot neutron, 512k flash, 128k sram
USBD_VID_INTOROBOT=0x1D50
USBD_PID_DFU=0x607F
USBD_PID_CDC=0x607D
DEFAULT_PRODUCT_ID=1
endif

ifeq ("$(PLATFORM_ID)","2") # gcc
ARCH=gcc
PLATFORM=gcc
PLATFORM_NAME=gcc
PLATFORM_MCU=gcc
PRODUCT_DESC=GCC xcompile
# explicitly exclude platform headers
INTOROBOT_NO_PLATFORM=1
DEFAULT_PRODUCT_ID=2
endif


ifeq ("$(PLATFORM_ID)","3") #neutron-net
ARCH=xtensa
PLATFORM=neutron-net
PLATFORM_NAME=neutron-net
PLATFORM_MCU=ESP8266
PRODUCT_DESC=ESP8266 4MB Flash
DEFAULT_PRODUCT_ID=3
endif

ifeq ("$(PLATFORM_ID)","4") #electron
ARCH=xtensa
PLATFORM=electron
PLATFORM_NAME=electron
PLATFORM_MCU=ESP8266
PRODUCT_DESC=IntoRobot electron, 4MB Flash
DEFAULT_PRODUCT_ID=4
endif

ifeq ("$(PLATFORM_ID)","60000") #newhal
PLATFORM=newhal
# needed for conditional compilation of some stm32 specific files
STM32_DEVICE=newhalcpu
# used to define the sources in hal/src/new-hal
PLATFORM_NAME=newhal
# define MCU-specific platform defines under platform/MCU/new-hal
PLATFORM_MCU=newhal-mcu
PLATFORM_NET=not-defined
PRODUCT_DESC=Test platform for producing a new HAL implementation
USBD_VID_INTOROBOT=0x1D50
USBD_PID_DFU=0x607F
USBD_PID_CDC=0x607D
DEFAULT_PRODUCT_ID=60000
endif

ifeq ("$(PLATFORM_NAME)","atom")
    PLATFORM_DFU ?= 0x08000000
else
ifeq ("$(PLATFORM_NAME)","neutron")
    PLATFORM_DFU ?= 0x08020000
endif
endif

ifeq ("$(PLATFORM_MCU)","")
$(error PLATFORM_MCU not defined. Check platform id $(PLATFORM_ID))
endif

# lower case version of the STM32_DEVICE string for use in filenames
STM32_DEVICE_LC  = $(shell echo $(STM32_DEVICE) | tr A-Z a-z)

ifdef STM32_DEVICE
# needed for conditional compilation of syshealth_hal.h
CDEFINES += -DSTM32_DEVICE
# needed for conditional compilation of some stm32 specific files
CDEFINES += -D$(STM32_DEVICE)
endif

ifeq ("$(PRODUCT_ID)","")
# ifeq (,$(submake))
# $(info PRODUCT_ID not defined - using default: $(DEFAULT_PRODUCT_ID))
# endif
PRODUCT_ID = $(DEFAULT_PRODUCT_ID)
endif

ifeq ("$(ARCH)","arm")
PLATFORM_THREADING ?= 0
CDEFINES += -DPLATFORM_THREADING=$(PLATFORM_THREADING)
CDEFINES += -DPLATFORM_ID=$(PLATFORM_ID) -DPLATFORM_NAME=$(PLATFORM_NAME)

CDEFINES += -DUSBD_VID_INTOROBOT=$(USBD_VID_INTOROBOT)
CDEFINES += -DUSBD_PID_DFU=$(USBD_PID_DFU)
CDEFINES += -DUSBD_PID_CDC=$(USBD_PID_CDC)
endif

MODULE_FUNCTION_NONE            :=0
MODULE_FUNCTION_RESOURCE        :=1
MODULE_FUNCTION_BOOTLOADER      :=2
MODULE_FUNCTION_MONO_FIRMWARE   :=3
MODULE_FUNCTION_SYSTEM_PART     :=4
MODULE_FUNCTION_USER_PART       :=5


endif