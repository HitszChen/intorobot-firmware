#ifndef __FILE_UPGRADE_H__
#define __FILE_UPGRADE_H__

#define SPI_FLASH_SEC_SIZE      4096       //Flash 扇区大小
#define LIMIT_ERASE_SIZE		0x10000    //Flash 擦除扇区大小限制


#define UPGRADE_FLAG_IDLE       0x00       //更新空闲
#define UPGRADE_FLAG_START      0x01       //更新开始
#define UPGRADE_FLAG_FINISH     0x02       //更新完成


enum update_file_type {
    ONLINE_APP_FILE,      //升级在线编程程序
    WIFI_FILE,            //升级wifi应用程序
    DEFAULT_APP_FILE      //升级默认应用程序
};


typedef void (*upgrade_states_check_callback)(void * arg);

//#define UPGRADE_SSL_ENABLE

struct upgrade_server_info {
    uint8 ip[4];
    uint16 port;

    uint8 upgrade_flag;

    uint8 pre_version[16];
    uint8 upgrade_version[16];

    char md5[36]; //文件md5码 32个字节  asc码

    uint32 check_times;
    uint8 *url;

    upgrade_states_check_callback check_cb;
    struct espconn *pespconn;
};

#define FILE_INFO_MAGIC 	 0xeb001000
#define FILE_INFO_MAGIC_MASK 0xfffff000

struct upgrade_file_info {
    uint32 magic;
    uint32 file_size;       //文件大小
    uint32 file_start_sec;  //文件起始扇区
    uint32 crc32;
};
/*
 *  20k  bootloader
 *  748k esp8266在线编程
 *  224k 默认应用程序
 *  32k  用户参数区
 *
 *  20k  下载文件属性记录区
 *  748k esp8266 下载缓冲区
 *  224k 默认应用程序 下载缓冲区
 *  32k  stm32 bootloader 下载缓冲区
 **
 */

#define WIFI_BOOT_SEC_START              0    //WIFI BOOT占用5个扇区
#define WIFI_APP_SEC_START               5    //WIFI APP运行起始扇区
#define DEFAPP_SEC_START                 192  //stm32默认程序存放起始扇区


#define UPDATE_FILE_INFO_SEC_START       512  //512 下载文件属性区

#define UPDATE_CACHE_WIFIAPP_SEC_START    261  //256+5    WIFI固件和在线应用程序下载扇区地址
#define UPDATE_CACHE_DEFAPP_SEC_START     448  //256+192  默认固件下载扇区地址


#define UPDATE_CACHE_DEFAPP_SEC_NUM       56   //64-8  8个扇区用于用户参数区 默认固件下载扇区总个数
#define UPDATE_CACHE_WIFIAPP_SEC_NUM      187  //192 - 5WIFI固件和在线应用程序下载扇区总个数


struct upgrade_param {
    uint32 fw_bin_addr;

    uint32 fw_bin_sec;
    uint32 fw_bin_sec_num;

    uint32 fw_bin_sec_earse;

    uint32 extra;

    uint8 save[4];

    uint8 *buffer;
};



void system_upgrade_init();
void system_upgrade_deinit();
bool system_upgrade(uint8 *data, uint16 len);

bool system_upgrade_start(struct upgrade_server_info *server);



//uint32_t ICACHE_FLASH_ATTR crc_update(uint32_t crc, const uint8_t *data, size_t length);
int ICACHE_FLASH_ATTR file_info_read(struct upgrade_file_info *file_info);
int ICACHE_FLASH_ATTR file_info_write(struct upgrade_file_info *file_info);
void ICACHE_FLASH_ATTR file_info_clear(void);





#endif
