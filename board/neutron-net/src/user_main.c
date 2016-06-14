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
#include "at_ex_command.h"
#include "upgrade.h"


extern enum update_file_type update_file;

at_funcationType at_custom_cmd[] = {
    {CMD_GETVERSION, 9, NULL, at_queryCmdGetVersion, NULL, NULL},
    {CMD_DOWN_FILE, 9, NULL, NULL, at_setupCmdDownFile, NULL},
    {CMD_GET_FILE_SIZE, 12, NULL, NULL, at_setupCmdGetFileSize, NULL},
    {CMD_GET_FILE_PACKET, 14, NULL, NULL, at_setupCmdGetFilePacket, NULL},
    {CMD_GET_FILE_ALL, 11, NULL, NULL, at_setupCmdGetFileAll, NULL}
};

void user_init(void)
{
    char buf[64] = {0};

    at_customLinkMax = 5;
    at_init();
    uart_init(460800, 115200);
    os_sprintf(buf,"compile time:%s %s",__DATE__,__TIME__);
    at_set_custom_info(buf);
    at_port_print("\r\nready\r\n");

    at_cmd_array_regist(&at_custom_cmd[0], sizeof(at_custom_cmd)/sizeof(at_custom_cmd[0]));
}

