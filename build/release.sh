#! /bin/bash

PLATFORM_ID=$1
PLATFORM=$2

BUILD_DIR=$(cd "$(dirname "$0")"; pwd)
PROJECT_DIR=$BUILD_DIR/..

rm -rf $BUILD_DIR/target/board/platform-$PLATFORM_ID
rm -rf $BUILD_DIR/target/platform/platform-$PLATFORM_ID
rm -rf $BUILD_DIR/release/intorobot-online/$PLATFORM

OUT=$BUILD_DIR/release/intorobot-online/$PLATFORM
LINKER=$OUT/linker
STARTUP=$OUT/startup
INC=$OUT/inc
INC_BOARD=$INC/inc_board
INC_MCU=$INC/inc_mcu
LIB=$OUT/lib

mkdir -p $OUT
mkdir -p $LINKER
mkdir -p $STARTUP
mkdir -p $INC
mkdir -p $INC_BOARD
mkdir -p $INC_MCU
mkdir -p $LIB

cd $PROJECT_DIR
#make -s PLATFORM_ID=$PLATFORM_ID clean all COMPILE_LTO=n
make -s PLATFORM_ID=$PLATFORM_ID clean all COMPILE_LTO=n

# cp lib
cp $BUILD_DIR/target/board/platform-$PLATFORM_ID/libboard.a $LIB/ -rf
cp $BUILD_DIR/target/platform/platform-$PLATFORM_ID/libplatform.a $LIB/ -rf 

# cp startup.o
cp $BUILD_DIR/target/main/platform-$PLATFORM_ID/startup/arm/startup_*.o $STARTUP/ -rf

# cp board header files
cp $PROJECT_DIR/board/$PLATFORM/inc/* $INC_BOARD/ -rf

if [ $PLATFORM == "atom" ]; then
    # cp linker
    cp $BUILD_DIR/linker/arm/linker_stm32f10x_md.ld $LINKER/
    # cp mcu header files
    cp $PROJECT_DIR/platform/MCU/STM32F1xx/CMSIS/Device/ST/Include/* $INC_MCU -rf
    cp $PROJECT_DIR/platform/MCU/STM32F1xx/CMSIS/Include/*  $INC_MCU -rf
    cp $PROJECT_DIR/platform/MCU/STM32F1xx/INTOROBOT_Firmware_Driver/inc/* $INC_MCU -rf
    cp $PROJECT_DIR/platform/MCU/STM32F1xx/STM32_StdPeriph_Driver/inc/* $INC_MCU -rf
    cp $PROJECT_DIR/platform/MCU/STM32F1xx/STM32_USB_Device_Driver/inc/* $INC_MCU -rf

    # remove include.mk
    rm $INC_BOARD/include.mk -rf
    rm $INC_MCU/include.mk -rf



elif [ $PLATFORM == "neutron" ]; then
    # cp linker
    cp $BUILD_DIR/linker/arm/linker_stm32f411xe_boot.ld $LINKER/

    # cp mcu header files
    cp $PROJECT_DIR/platform/MCU/STM32F4xx/CMSIS/Device/ST/STM32F4xx/Include/* $INC_MCU -rf
    cp $PROJECT_DIR/platform/MCU/STM32F4xx/CMSIS/Include/*  $INC_MCU -rf
    cp $PROJECT_DIR/platform/MCU/STM32F4xx/INTOROBOT_Firmware_Driver/inc/* $INC_MCU -rf
    cp $PROJECT_DIR/platform/MCU/STM32F4xx/STM32F4xx_HAL_Driver/Inc/* $INC_MCU -rf
    cp $PROJECT_DIR/platform/MCU/STM32F4xx/STM32_USB_Device_Library/Class/CDC/Inc/* $INC_MCU -rf
    cp $PROJECT_DIR/platform/MCU/STM32F4xx/STM32_USB_Device_Library/Class/DFU/Inc/* $INC_MCU -rf
    cp $PROJECT_DIR/platform/MCU/STM32F4xx/STM32_USB_Device_Library/Core/Inc/* $INC_MCU -rf

    # cp freeRTOS header files
    cp $PROJECT_DIR/board/neutron/src/FreeRTOS/Source/CMSIS_RTOS/*.h $INC_BOARD -rf
    cp $PROJECT_DIR/board/neutron/src/FreeRTOS/Source/include/* $INC_BOARD -rf
    cp $PROJECT_DIR/board/neutron/src/FreeRTOS/Source/portable/GCC/ARM_CM4F/*.h $INC_BOARD -rf

    # cp libPDMFilter_CM$_GCC.a
    cp $PROJECT_DIR/board/neutron/lib/libPDMFilter_CM4_GCC.a $LIB/

    # remove include.mk
    rm $INC_BOARD/include.mk -rf
    rm $INC_MCU/include.mk -rf

#elif [ $PLATFORM == "electron" ]; then
    # cp linker

    # cp the header files
fi





