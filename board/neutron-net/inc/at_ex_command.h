
#ifndef __AT_EX_COMMAND_H__
#define __AT_EX_COMMAND_H__



#define CMD_GETVERSION          "+MOLMCGMR"         //获取esp8266版本信息
#define CMD_DOWN_FILE           "+DOWNFILE"         //在线升级 stm32应用程序
#define CMD_GET_FILE_SIZE       "+GETFILESIZE"      //获取应用程序大小
#define CMD_GET_FILE_PACKET     "+GETFILEPACKET"    //获取应用程序数据包
#define CMD_GET_FILE_ALL        "+GETFILEALL"       //获取应用程序整个数据

//at get file all
struct loadfile{
    int file_sec_size;
    int packet_size;
    int file_type;
    int packet_index;
};


void ICACHE_FLASH_ATTR at_queryCmdGetVersion(uint8_t id);
void ICACHE_FLASH_ATTR at_setupCmdDownFile(uint8_t id, char *pPara);
void ICACHE_FLASH_ATTR at_setupCmdGetFileSize(uint8_t id, char *pPara);
void ICACHE_FLASH_ATTR at_setupCmdGetFilePacket(uint8_t id, char *pPara);
void ICACHE_FLASH_ATTR at_setupCmdGetFileAll(uint8_t id, char *pPara);



#endif
