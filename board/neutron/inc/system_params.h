/**
 ******************************************************************************
 * @file     : wiring_flash_memory.h
 * @author   : robin
 * @version	 : V1.0.0
 * @date     : 6-December-2014
 * @brief    :
 ******************************************************************************
  Copyright (c) 2013-2014 IntoRobot Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */

#ifndef SYSTEM_PARAMS_H_
#define SYSTEM_PARAMS_H_

#include "variant.h"

//参数区大小不能变动
typedef struct __attribute__((packed)) system_flags
{
    uint32_t header;          // 系统标志参数区标志  固定为0x5AA5F66F
    uint32_t version;         // bootloader版本号
    uint8_t  boot_flag;       // bootloader处理标志 0:正常启动  1:进入默认程序恢复  2:进入esp8266串口转发  3:恢复出厂  4:进入在线应用升级
    uint8_t  initparam_flag;  // 参数初始化标志  0:应用参数不初始化  1:应用参数恢复出厂设置(保留密钥)  2:应用参数恢复出厂设置(不保留密钥)
    uint8_t  reserved[61];    // 参数预留区 每添加一个参数，预留区大小减1
    uint8_t  crc;             // 系统标志参数区校验字节
}system_flags_t;


typedef struct __attribute__((packed)) system_params
{
    system_flags_t system_flags;
    uint32_t version;              // 系统应用参数版本号
    uint32_t module_version;       // WIFI模块版本号
    uint32_t fw_version;           // 固件版本号
    uint8_t  reserverd[64];        // 版本号预留区 每添加一个版本号，预留区大小减1
    uint8_t  config_flag;          // 是否进入配置模式  0:不进入  1:进入
    uint8_t  ota_flag;             // 是否上报在线升级成功状态   0:不上报  1:上报
    uint8_t  reset_flag;           // 是否上报重启成功状态   0:不上报  1:上报
    uint8_t  at_mode;              // 是否已经灌装密钥  0:未灌装 1:已经灌装
    uint8_t  sv_select;            // 是否选择默认服务参数  0:使用 1:不使用
    uint8_t  reserverd1[31];       // 版本号预留区 每添加一个版本号，预留区大小减1
    char   device_id[52];          // 设备序列号
    char  access_token[52];        // 设备access_token
    char  sv_domain[52];           // 服务器域名
    int sv_port;                   // 服务器端口
    char  dw_domain[52];           // 服务器下载域名
    float    zone;                 // 核心板所在时区。用于实时时钟。

    uint8_t  reserved2[599];      // 参数预留区 每添加一个参数，预留区大小减1
    uint8_t  crc;                  // 系统应用参数区校验字节
} system_params_t;


#ifdef __cplusplus
 extern "C" {
#endif


uint8_t crcSum( const uint8_t *data, uint32_t len);
void initSystemFlags(system_flags_t *psystem_flags);
void loadSystemFlags(system_flags_t *psystem_flags);
void saveSystemFlags(system_flags_t *psystem_flags);
void initSystemParams(system_params_t *psystem_params);
void loadSystemParams(system_params_t *psystem_params);
void saveSystemParams(system_params_t *psystem_params);
void initFacSystemParams(system_params_t *psystem_params);

#ifdef __cplusplus
}
#endif


#endif /*SYSTEM_PARAMS_H_*/


