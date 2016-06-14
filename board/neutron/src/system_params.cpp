#include "system_params.h"
#include "wiring_flash_memory.h"
#include <string.h>
#include <stdio.h>
#include "lib_system_all.h"

/*初始化系统标志参数区*/
uint8_t crcSum( const uint8_t *data, uint32_t len)
{
    uint8_t sum = 0;
    uint32_t n;
    for( n=0; n < len;)
    {
        sum += data[n++];
    }
    return sum;
}

/*初始化系统参数区*/
void initSystemParams(system_params_t *psystem_params)
{
    memset((uint8_t *)psystem_params, 0, sizeof(system_params_t));
    psystem_params->system_flags.header = 0x5aa5f66f;
    psystem_params->config_flag = 1;
    saveSystemParams(psystem_params);
}

/*初始化系统参数区 保留密钥参数*/
void initFacSystemParams(system_params_t *psystem_params)
{
    uint8_t  at_mode;                  // 是否已经灌装密钥  0:未灌装 1:已经灌装
    uint8_t  device_id[52]={0};        // 设备序列号
    uint8_t  access_token[52]={0};     // 设备access_token

    at_mode = psystem_params->at_mode;
    memcpy(device_id, psystem_params->device_id, sizeof(psystem_params->device_id));
    memcpy(access_token, psystem_params->access_token, sizeof(psystem_params->access_token));

    initSystemParams(psystem_params);

    psystem_params->at_mode = at_mode;
    memcpy(psystem_params->device_id, device_id, sizeof(psystem_params->device_id));
    memcpy(psystem_params->access_token, access_token, sizeof(psystem_params->access_token));

    saveSystemParams(psystem_params);
}

/*加载系统参数区*/
void loadSystemParams(system_params_t *psystem_params)
{
    memset(psystem_params, 0, sizeof(system_params_t));
    SystemReadArgument(SYSTEM_ARGUMENT_ADDRESS, (uint16_t *)psystem_params, sizeof(system_params_t)/2);
    if(( 0x5aa5f66f != psystem_params->system_flags.header )
            ||(psystem_params->system_flags.crc != crcSum((const uint8_t *)&psystem_params->system_flags, sizeof(system_flags_t)-1))
            ||(psystem_params->crc != crcSum((const uint8_t *)&psystem_params->version, sizeof(system_params_t)-sizeof(system_flags_t)-1)))
    {
        initSystemParams(psystem_params);
    }
}

/*保存系统参数区*/
void saveSystemParams(system_params_t *psystem_params)
{
    psystem_params->system_flags.crc = crcSum((const uint8_t *)&psystem_params->system_flags, sizeof(system_flags_t)-1);
    psystem_params->crc = crcSum((const uint8_t *)&psystem_params->version, sizeof(system_params_t)-sizeof(system_flags_t)-1);
    SystemWriteArgument(SYSTEM_ARGUMENT_ADDRESS, (uint16_t *)psystem_params, sizeof(system_params_t)/2);
}



