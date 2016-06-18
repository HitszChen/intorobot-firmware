/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2015/1/23, v1.0 create this file.
*******************************************************************************/

#include "osapi.h"
#include "at_custom.h"
#include "user_interface.h"
#include "user_config.h"


struct eboot_command cmd_info;

#define CMD_GETVERSION          "+GETVERSION"       //获取wifi程序版本信息
#define CMD_DOWN_PROGRESS       "+DOWNPROGRESS"     //获取文件下载进度
#define CMD_DOWN_FILE           "+DOWNFILE"         //在线编程文件  wifi应用升级 默认应用程序
#define CMD_GET_FILE_SIZE       "+GETFILESIZE"      //获取应用程序大小
#define CMD_GET_FILE_PACKET     "+GETFILEPACKET"    //获取应用程序数据包
#define CMD_ESP8266_UPDATE      "+ESP8266UPDATE"    //升级esp8266程序



at_funcationType at_custom_cmd[] = {
    {CMD_GETVERSION, 11, NULL, at_queryCmdGetVersion, NULL, NULL},
    {CMD_DOWN_PROGRESS, 13, NULL, at_queryCmdGetDownProgress, NULL, NULL},
    {CMD_DOWN_FILE, 9, NULL, NULL, at_setupCmdDownFile, NULL},
    {CMD_GET_FILE_SIZE, 12, NULL, NULL, at_setupCmdGetFileSize, NULL},
    {CMD_GET_FILE_PACKET, 14, NULL, NULL, at_setupCmdGetFilePacket, NULL},
    {CMD_ESP8266_UPDATE, 14, NULL, NULL, at_setupCmdEsp8266Update, NULL},
};

void user_init(void)
{
    char buf[64] = {0};

    at_customLinkMax = 5;
    eboot_command_read(&cmd_info);  //读取命令信息
    at_init();
    //uart_init(460800, 115200);
    os_sprintf(buf,"compile time:%s %s",__DATE__,__TIME__);
    at_set_custom_info(buf);
    at_port_print("\r\nready\r\n");

    at_cmd_array_regist(&at_custom_cmd[0], sizeof(at_custom_cmd)/sizeof(at_custom_cmd[0]));
}

