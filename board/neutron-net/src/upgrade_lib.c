#include "ets_sys.h"
#include "spi_flash.h"

//#include "net80211/ieee80211_var.h"
#include "lwip/mem.h"

#include "upgrade.h"


LOCAL struct upgrade_param *upgrade;
LOCAL struct upgrade_file_info *file_info;
LOCAL enum update_file_type update_file = ONLINE_APP_FILE;

extern SpiFlashChip *flashchip;

/******************************************************************************
 * FunctionName : system_upgrade_internal
 * Description  : a
 * Parameters   :
 * Returns      :
*******************************************************************************/
LOCAL bool ICACHE_FLASH_ATTR
system_upgrade_internal(struct upgrade_param *upgrade, uint8 *data, uint16 len)
{
    bool ret = false;
    if(data == NULL || len == 0)
    {
    	return true;
    }
    upgrade->buffer = (uint8 *)os_zalloc(len + upgrade->extra);

    os_memcpy(upgrade->buffer, upgrade->save, upgrade->extra);
    os_memcpy(upgrade->buffer + upgrade->extra, data, len);

    len += upgrade->extra;
    upgrade->extra = len & 0x03;
    len -= upgrade->extra;

    os_memcpy(upgrade->save, upgrade->buffer + len, upgrade->extra);

    do {
        if (upgrade->fw_bin_addr + len >= (upgrade->fw_bin_sec + upgrade->fw_bin_sec_num) * SPI_FLASH_SEC_SIZE) {
            break;
        }

        if (len > SPI_FLASH_SEC_SIZE) {

        } else {
			os_printf("%x %x\n",upgrade->fw_bin_sec_earse,upgrade->fw_bin_addr);
            /* earse sector, just earse when first enter this zone */
            if (upgrade->fw_bin_sec_earse != (upgrade->fw_bin_addr + len) >> 12) {
                upgrade->fw_bin_sec_earse = (upgrade->fw_bin_addr + len) >> 12;
                spi_flash_erase_sector(upgrade->fw_bin_sec_earse);
				os_printf("%x\n",upgrade->fw_bin_sec_earse);
            }
        }

        if (spi_flash_write(upgrade->fw_bin_addr, (uint32 *)upgrade->buffer, len) != SPI_FLASH_RESULT_OK) {
            break;
        }

        ret = true;
        upgrade->fw_bin_addr += len;
    } while (0);

    os_free(upgrade->buffer);
    upgrade->buffer = NULL;
    return ret;
}

/******************************************************************************
 * FunctionName : system_upgrade
 * Description  : a
 * Parameters   :
 * Returns      :
*******************************************************************************/
bool ICACHE_FLASH_ATTR
system_upgrade(uint8 *data, uint16 len)
{
    bool ret;

    ret = system_upgrade_internal(upgrade, data, len);

    return ret;
}

/******************************************************************************
 * FunctionName : system_upgrade_init
 * Description  : a
 * Parameters   :
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR
system_upgrade_init(void)
{
    uint32 user_bin2_start;
    uint8 flash_buf[4];
    uint8 high_half;

    spi_flash_read(0, (uint32 *)flash_buf, 4);
    high_half = (flash_buf[3] & 0xF0) >> 4;

    if (upgrade == NULL) {
        upgrade = (struct upgrade_param *)os_zalloc(sizeof(struct upgrade_param));
    }

    if (file_info == NULL) {
        file_info = (struct upgrade_file_info *)os_zalloc(sizeof(struct upgrade_file_info));
    }

    system_upgrade_flag_set(UPGRADE_FLAG_IDLE);

    if (DEFAULT_APP_FILE == update_file) {
		upgrade->fw_bin_sec = UPDATE_CACHE_DEFAPP_SEC_START;
		upgrade->fw_bin_sec_num = UPDATE_CACHE_DEFAPP_SEC_NUM;
	} else {
		upgrade->fw_bin_sec = UPDATE_CACHE_WIFIAPP_SEC_START;
		upgrade->fw_bin_sec_num = UPDATE_CACHE_WIFIAPP_SEC_NUM;
	}
    upgrade->fw_bin_addr = upgrade->fw_bin_sec * SPI_FLASH_SEC_SIZE;
}

/******************************************************************************
 * FunctionName : system_upgrade_deinit
 * Description  : a
 * Parameters   :
 * Returns      :
*******************************************************************************/
void ICACHE_FLASH_ATTR
system_upgrade_deinit(void)
{
    os_free(upgrade);
    os_free(file_info);
    upgrade = NULL;
}

uint32_t ICACHE_FLASH_ATTR
crc32_update(uint32_t crc, const uint8_t *data, size_t length)
{
    uint32_t i;
    bool bit;
    uint8_t c;

    while (length--) {
        c = *data++;
        for (i = 0x80; i > 0; i >>= 1) {
            bit = crc & 0x80000000;
            if (c & i) {
                bit = !bit;
            }
            crc <<= 1;
            if (bit) {
                crc ^= 0x04c11db7;
            }
        }
    }
    return crc;
}

uint32_t ICACHE_FLASH_ATTR
file_info_calculate_crc32(struct upgrade_file_info *file_info)
{
    return crc32_update(0xffffffff, (const uint8_t*)file_info,
                      offsetof(struct upgrade_file_info, crc32));
}

int ICACHE_FLASH_ATTR
file_info_read(struct upgrade_file_info *file_info)
{
    if (SPI_FLASH_RESULT_OK != spi_flash_read(UPDATE_FILE_INFO_SEC_START * SPI_FLASH_SEC_SIZE, (uint32 *)file_info, sizeof(struct upgrade_file_info))) {
        return 1;
    }
    uint32_t crc32 = file_info_calculate_crc32(file_info);
    if (file_info->magic & FILE_INFO_MAGIC_MASK != FILE_INFO_MAGIC ||
        file_info->crc32 != crc32) {
        return 1;
    }
    return 0;
}

int ICACHE_FLASH_ATTR
file_info_write(struct upgrade_file_info *file_info)
{
    file_info->magic = FILE_INFO_MAGIC;
    file_info->crc32 = file_info_calculate_crc32(file_info);

    if( SPI_FLASH_RESULT_OK != spi_flash_erase_sector(UPDATE_FILE_INFO_SEC_START)){
        return 1;
    }

    if (SPI_FLASH_RESULT_OK != spi_flash_write(UPDATE_FILE_INFO_SEC_START * SPI_FLASH_SEC_SIZE, (uint32 *)file_info, sizeof(struct upgrade_file_info))) {
        return 1;
    }
    return 0;
}

void ICACHE_FLASH_ATTR
file_info_clear(void)
{
    struct upgrade_file_info file_info;
    os_memset((uint32 *)&file_info, 0, sizeof(struct upgrade_file_info));
    file_info_write(&file_info);
}


