#ifndef __AT_EX_COMMAND_H__
#define __AT_EX_COMMAND_H__



#define CMD_GETVERSION          "+GETVERSION"       //获取wifi程序版本信息
#define CMD_DOWN_PROGRESS       "+DOWNPROGRESS"     //获取文件下载进度
#define CMD_DOWN_FILE           "+DOWNFILE"         //在线编程文件  wifi应用升级 默认应用程序
#define CMD_GET_FILE_SIZE       "+GETFILESIZE"      //获取应用程序大小
#define CMD_GET_FILE_PACKET     "+GETFILEPACKET"    //获取应用程序数据包
#define CMD_ESP8266_UPDATE      "+ESP8266UPDATE"    //升级esp8266程序


void ICACHE_FLASH_ATTR at_queryCmdGetVersion(uint8_t id);
void ICACHE_FLASH_ATTR at_queryCmdGetDownProgress(uint8_t id);
void ICACHE_FLASH_ATTR at_setupCmdDownFile(uint8_t id, char *pPara);
void ICACHE_FLASH_ATTR at_setupCmdGetFileSize(uint8_t id, char *pPara);
void ICACHE_FLASH_ATTR at_setupCmdGetFilePacket(uint8_t id, char *pPara);
void ICACHE_FLASH_ATTR at_setupCmdEsp8266Update(uint8_t id, char *pPara);


#endif
