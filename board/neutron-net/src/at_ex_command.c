/**
 ******************************************************************************
 * @file
 * @authors
 * @version V1.0.0
 * @date    2014_11_13
 * @brief
 ******************************************************************************
 */
/****************************************************************************
 *Private Included Files
 ****************************************************************************/
#include "c_types.h"
#include "user_interface.h"
#include "espconn.h"
#include "mem.h"
#include "osapi.h"
#include "upgrade.h"
#include "at_ex_command.h"
#include "crc16.h"

#define pheadbuffer "Connection: keep-alive\r\n\
Cache-Control: no-cache\r\n\
User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.101 Safari/537.36 \r\n\
Accept: */*\r\n\
Accept-Encoding: gzip,deflate\r\n\
Accept-Language: zh-CN,eb-US;q=0.8\r\n\r\n"

struct espconn *pespconn = NULL;
struct upgrade_server_info *upServer = NULL;

static ip_addr_t host_ip;
extern enum update_file_type update_file;

static os_timer_t at_get_file_timer;
struct loadfile *ploadfile = NULL;


#define UART0   0
#define UART1   1


/******************************************************************************
 * FunctionName : uart0_tx_buffer
 * Description  : use uart0 to transfer buffer
 * Parameters   : uint8 *buf - point to send buffer
 *                uint16 len - buffer len
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR
uart0_tx_buffer(uint8 *buf, uint16 len)
{
    uint16 i;
    for (i = 0; i < len; i++)
    {
        uart_tx_one_char(UART0, buf[i]);
    }
}


//升级回调函数
LOCAL void ICACHE_FLASH_ATTR
at_upDateApp_rsp(void *arg)
{
    char output[64] = {0};
    struct upgrade_server_info *server = arg;

    os_memset(output, 0, sizeof(output));

    if(server->upgrade_flag == true)
    {
        os_printf("device_upgrade_success\r\n");
        os_sprintf(output,"%s:0\r\n", CMD_DOWN_FILE);     //下载成功
       //system_upgrade_reboot();
    }
    else
    {
        os_printf("device_upgrade_failed\r\n");
        os_sprintf(output,"%s:1\r\n", CMD_DOWN_FILE);     //下载成功
    }
    at_port_print(output);//下载失败
    at_response_ok();

    os_free(server->url);
    server->url = NULL;
    os_free(server);
    server = NULL;
}

//通过host name回去ip回调函数
LOCAL void ICACHE_FLASH_ATTR
upServer_dns_found(const char *name, ip_addr_t *ipaddr, void *arg)
{
    struct espconn *pespconn = (struct espconn *) arg;
    char output[64] = {0};

    os_printf("upServer_dns_found\r\n");
    if(ipaddr == NULL)
    {
        at_upDateApp_rsp(upServer);
        os_memset(output, 0, sizeof(output));
        os_sprintf(output,"%s:1\r\n", CMD_DOWN_FILE);
        at_port_print(output);//下载失败
        at_response_ok();
        return;
    }
    os_memcpy(upServer->ip, &ipaddr->addr, 4);
    system_upgrade_start(upServer);
}

/*
获取版本号
指令:AT+MOLMCGMR
返回：
WIFI version:1.0.0
OK
*/
    void ICACHE_FLASH_ATTR
at_queryCmdGetVersion(uint8_t id){
    uint8 buffer[64] = {0};

    os_sprintf(buffer, "WIFI version:%s\r\n", WIFIVERSION);
    at_port_print(buffer);
    at_response_ok();
}

/*
在线升级应用程序
AT+DOWNFILE="http://112.124.117.64/v1/bins/?dwn_token=8b66c607d5516e3b79f56afd37471a24","955d2d630866ba99711614cccf2b5ba2"
返回：
+DOWNFILE:<status>,<progress>  0:下载成功 1:下载失败 2.正在下载
OK
*/
    void ICACHE_FLASH_ATTR
at_setupCmdDownFile(uint8_t id, char *pPara){
    char hostname[64] = {0};
    char httppara[64] = {0};
    char md5para[64] = {0};

    if (*pPara++ != '=') // skip '='
    {
        at_response_error();
        return;
    }

    //获取第1个参数 hostname
    at_data_str_copy(hostname, &pPara, sizeof(hostname));
    //os_printf("hostname = %s pPara=%s \n", hostname, pPara);
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //获取第2个参数 http para
    at_data_str_copy(httppara, &pPara, sizeof(httppara));
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    os_printf("httppara = %s\n", httppara);
    //获取第3个参数 md5
    at_data_str_copy(md5para, &pPara, sizeof(md5para));
    os_printf("md5para = %s\n", md5para);

    upServer = (struct upgrade_server_info *)os_zalloc(sizeof(struct upgrade_server_info));
    upServer->upgrade_version[5] = '\0';
    upServer->port = 80;
    upServer->check_cb = at_upDateApp_rsp;
    upServer->check_times = 20000;

    if(upServer->url == NULL)
    {
        upServer->url = (uint8 *) os_zalloc(1024);
    }

    os_sprintf(upServer->url,
            "GET %s HTTP/1.0\r\nHost: %s\r\n"pheadbuffer"",
            httppara, hostname);

    host_ip.addr = ipaddr_addr(hostname);
    if (host_ip.addr != IPADDR_NONE)
    {
        os_memcpy(upServer->ip, &host_ip.addr, 4);
        os_strcpy(upServer->md5, md5para);
        system_upgrade_start(upServer);
    }
    else
    {
        pespconn = (struct espconn *)os_zalloc(sizeof(struct espconn));
        espconn_gethostbyname(pespconn, hostname, &host_ip, upServer_dns_found);
    }

}
/*
获取应用程序文件大小
cmd:AT+GETFILESIZE=16384,1  第1个参数:每个包的大小   第二个参数：文件类型     0 在线应用文件   1 默认应用文件
返回：
+GETFILESIZE:14             第1个参数:包个数
*/
    void ICACHE_FLASH_ATTR
at_setupCmdGetFileSize(uint8_t id, char *pPara){
    char buffer[64] = {0};
    int packet_size = 1024, file_type = 0, err = 0;
    struct upgrade_file_info file_info;
    uint32_t file_sec_size;

    if (*pPara++ != '=') // skip '='
    {
        at_response_error();
        return;
    }
    //get the first parameter digit
    at_get_next_int_dec(&pPara, &packet_size, &err);
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //get the second parameter digit
    at_get_next_int_dec(&pPara, &file_type, &err);
    if (*pPara != '\r') {
        at_response_error();
        return;
    }
    switch(file_type)
    {
        case 0: //在线下载应用文件
            if(file_info_read(&file_info)){
                at_response_error();
                break;
            }
        case 1: //默认应用文件
            if(file_type == 0){
                file_sec_size = (file_info.file_size + packet_size -1)/packet_size;
            }
            else{
                file_sec_size = (UPDATE_CACHE_DEFAPP_SEC_NUM * SPI_FLASH_SEC_SIZE + packet_size -1)/packet_size;
            }
            os_sprintf(buffer,"%s:%d\r\n", CMD_GET_FILE_SIZE ,file_sec_size);
            at_port_print(buffer);
            at_response_ok();
            break;
        default:
            at_response_error();
            break;
    }
}

//获取应用程序数据包
//cmd:AT+GETFILEPACKET=16384,0,1  第1个参数:每个包的大小    第2个参数:第几个包   第3个参数：文件类型     0 在线应用文件   1 默认应用文件
//返回：
//+GETFILEPACKET,16388:数据
    void ICACHE_FLASH_ATTR
at_setupCmdGetFilePacket(uint8_t id, char *pPara){
    char buffer[64] = {0};
    uint8_t *packet_buffer = NULL;
    int packet_size = 1024, packet_index = 0, file_type = 0, err = 0;
    struct upgrade_file_info file_info;
    uint32_t readaddr;

    if (*pPara++ != '=') // skip '='
    {
        at_response_error();
        return;
    }
    //get the first parameter digit
    at_get_next_int_dec(&pPara, &packet_size, &err);
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //get the second parameter digit
    at_get_next_int_dec(&pPara, &packet_index, &err);
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //get the third parameter digit
    at_get_next_int_dec(&pPara, &file_type, &err);
    if (*pPara != '\r') {
        at_response_error();
        return;
    }

    switch(file_type)
    {
        case 0: //在线下载应用文件
            if(file_info_read(&file_info)){
                at_response_error();
                break;
            }
        case 1: //默认应用文件
            ets_wdt_disable();
            packet_buffer = (uint8_t *)os_zalloc(packet_size + 2);
            if(file_type == 0){
                readaddr = UPDATE_CACHE_WIFIAPP_SEC_START * SPI_FLASH_SEC_SIZE + packet_size * packet_index;
            }
            else{
                readaddr = DEFAPP_SEC_START * SPI_FLASH_SEC_SIZE + packet_size * packet_index;
            }
            os_memset(packet_buffer, 0, sizeof(packet_buffer));
            if (SPI_FLASH_RESULT_OK == spi_flash_read(readaddr, (uint32 *)packet_buffer, packet_size)) {
                os_memset(buffer, 0, sizeof(buffer));
                os_sprintf(buffer,"%s,%d:",CMD_GET_FILE_PACKET, packet_size+2);
                //at_port_print(buffer);
                uart0_tx_buffer(buffer,strlen(buffer));

                crc_reset();
                crc_update_n(packet_buffer,packet_size);
                uint16_t crc16 = crc_get_reseult();
                os_memcpy(&packet_buffer[packet_size], (uint8 *)&crc16, 2);

                //发送一个包数据+16bitCRC
                uart0_tx_buffer(packet_buffer,packet_size+2);
                uart0_tx_buffer("\r\n",2);
            }
            else{
                at_response_error();
            }
            os_free(packet_buffer);
            packet_buffer = NULL;
            break;
        default:
            at_response_error();
            break;
    }
}

LOCAL void ICACHE_FLASH_ATTR
at_getfileall_timer_cb(void *arg)
{
    char buffer[64] = {0};
    uint8_t *packet_buffer;
    uint32_t readaddr;

    struct loadfile *ploadfile = arg;
    os_timer_disarm(&at_get_file_timer);

    if(ploadfile != NULL)
    {
        packet_buffer = (uint8_t *)os_zalloc(ploadfile->packet_size + 2);
        if(ploadfile->file_type == 0){
            readaddr = UPDATE_CACHE_WIFIAPP_SEC_START * SPI_FLASH_SEC_SIZE + ploadfile->packet_size * ploadfile->packet_index;
        }
        else{
            readaddr = DEFAPP_SEC_START * SPI_FLASH_SEC_SIZE + ploadfile->packet_size * ploadfile->packet_index;
        }
        os_memset(packet_buffer, 0, sizeof(packet_buffer));
        if (SPI_FLASH_RESULT_OK == spi_flash_read(readaddr, (uint32 *)packet_buffer, ploadfile->packet_size)) {
            os_memset(buffer, 0, sizeof(buffer));
            os_sprintf(buffer,"%s,%d,%d,%d:",CMD_GET_FILE_ALL, ploadfile->file_sec_size, ploadfile->packet_index, ploadfile->packet_size+2);
            //at_port_print(buffer);
            uart0_tx_buffer(buffer,strlen(buffer));

            crc_reset();
            crc_update_n(packet_buffer,ploadfile->packet_size);
            uint16_t crc16 = crc_get_reseult();
            os_memcpy(&packet_buffer[ploadfile->packet_size], (uint8 *)&crc16, 2);

            //发送一个包数据+16bitCRC
            uart0_tx_buffer(packet_buffer,ploadfile->packet_size+2);
            uart0_tx_buffer("\r\n",2);
        }
        ets_wdt_enable();
        os_free(packet_buffer);
        packet_buffer = NULL;
        ploadfile->packet_index++;
        if(ploadfile->packet_index < ploadfile->file_sec_size){
            os_timer_disarm(&at_get_file_timer);
            os_timer_setfn(&at_get_file_timer, (os_timer_func_t *)at_getfileall_timer_cb, ploadfile);
            os_timer_arm(&at_get_file_timer, 1, 0);
        }
    }
    else{
        at_response_error();
    }
}


//获取应用程序数据包
//cmd:AT+GETFILEALL=16384,1    第1个参数:每个数据段长度   第二个参数：文件类型     0 在线应用文件   1 默认应用文件
//返回：
//+GETFILEALL,14,0,16388:数据
    void ICACHE_FLASH_ATTR
at_setupCmdGetFileAll(uint8_t id, char *pPara){
    char buffer[64] = {0};
    uint8 packet_buffer[SPI_FLASH_SEC_SIZE*2 + 4];
    int packet_size = 0, file_type = 0, err = 0;
    struct upgrade_file_info file_info;
    uint32_t readaddr;
    uint32_t file_sec_size;

    if (*pPara++ != '=') // skip '='
    {
        at_response_error();
        return;
    }
    //get the first parameter digit
    at_get_next_int_dec(&pPara, &packet_size, &err);
    if (*pPara++ != ',') { // skip ','
        at_response_error();
        return;
    }
    //get the second parameter digit
    at_get_next_int_dec(&pPara, &file_type, &err);
    if (*pPara != '\r') {
        at_response_error();
        return;
    }

    switch(file_type)
    {
        case 0: //在线下载应用文件
            if(file_info_read(&file_info)){
                at_response_error();
                break;
            }
        case 1: //默认应用文件
            if(file_type == 0){
                file_sec_size = (file_info.file_size + packet_size -1)/packet_size;
            }
            else{
                file_sec_size = (UPDATE_CACHE_DEFAPP_SEC_NUM*SPI_FLASH_SEC_SIZE + packet_size -1)/packet_size;
            }
            ploadfile = (struct loadfile *)os_zalloc(sizeof(struct loadfile));
            ploadfile->file_sec_size = file_sec_size;
            ploadfile->packet_size = packet_size;
            ploadfile->packet_index = 0;
            ploadfile->file_type = file_sec_size;
            os_timer_disarm(&at_get_file_timer);
            os_timer_setfn(&at_get_file_timer, (os_timer_func_t *)at_getfileall_timer_cb, ploadfile);
            os_timer_arm(&at_get_file_timer, 1, 0);
            break;
        default:
            at_response_error();
            break;
    }
}

