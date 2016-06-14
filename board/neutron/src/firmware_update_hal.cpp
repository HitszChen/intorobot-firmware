//=================================================================================================================
//input
/****************************************************************************
 *Private Included Files
 ****************************************************************************/

#include <stdint.h>
#include "lib_system_all.h"
#include "lib_wifi_drv.h"


extern void mo_system_reboot_hal();

 //=================================================================================================================
//come true hidden
/************************************************************************************
* Private Types
************************************************************************************/


/************************************************************************************
* Private Variables
************************************************************************************/


/************************************************************************************
* Private Functions
************************************************************************************/


//=================================================================================================================
//come true export
/************************************************************************************
* Public Type
************************************************************************************/



/************************************************************************************
*  Public Functions
************************************************************************************/

/*
成功返回0

*/
//cmd:AT+MDSET="md5:32bytes"
//cmd:AT+DOWNFW="http://www.intorobot.com/v1/bin?dwn_token=32bytes"
int mo_FirmwareUpdateClass_st_firmware_down_hal(const char *domain, const char *param, const char * md5)
{
    String server_down_cmd = "AT+DOWNFILE=";
    server_down_cmd += "\"";
    server_down_cmd += domain;
    server_down_cmd += "\",\"";
    server_down_cmd += param;
    server_down_cmd += "\",\"";
    server_down_cmd += md5;
    server_down_cmd += "\"";

    MO_DEBUG(( server_down_cmd.c_str() ));

    char *server_download_cmd = const_cast<char *>(server_down_cmd.c_str());

    if (mo_drv_wifi_run_cmd( server_download_cmd, "+DOWNFILE:0", 10) )
    {
        intorobot_system_param.ota_flag = 1;
        saveSystemParams( &intorobot_system_param );

        MO_DEBUG(("Download Succeed"));
        return 0;
    }
    else
    {
        MO_ERROR(("Download Failed"));
        return 1;
    }
}


/*
成功返回0

*/

int mo_FirmwareUpdateClass_st_firmware_update_hal(void)
{
    MO_ERROR(("not support"));
    return 0;
}

/*
成功返回0

*/

int mo_FirmwareUpdateClass_st_firmware_isupdate_hal(void)
{
    MO_DEBUG(("ota_flag = %d\r\n",intorobot_system_param.ota_flag));
    if (1 == intorobot_system_param.ota_flag)
    {
        return 0;
    }
    return 1;
}

/*
成功返回0

*/

int mo_FirmwareUpdateClass_st_firmware_clearupdate_hal(void)
{
    // clear the ota flag
    intorobot_system_param.ota_flag = 0;
    saveSystemParams( &intorobot_system_param );
    return 0;

}

/*
成功返回0

*/

int mo_FirmwareUpdateClass_openwrt_firmware_version_hal(char *p_version)
{
    MO_ERROR(("not support"));
    return 0;
}



int mo_FirmwareUpdateClass_openwrt_firmware_down(const char *app_id, const char *server_addr)
{
    MO_ERROR(("not support"));
    return 0;
}



int mo_FirmwareUpdateClass_openwrt_firmware_update_hal(void)
{
    MO_ERROR(("not support"));
    return 0;
}


int mo_FirmwareUpdateClass_openwrt_firmware_isupdate(void)
{
    MO_ERROR(("not support"));
    return 0;

}


int mo_FirmwareUpdateClass_openwrt_firmware_clearupdate(void)
{
    MO_ERROR(("not support"));
    return 0;
}


int mo_FirmwareUpdateClass_st_system_reset_hal(void)
{
    intorobot_system_param.reset_flag = 1;
    saveSystemParams( &intorobot_system_param );
    mo_system_reboot_hal();
    MO_ERROR(("not support"));
    return 0;
}

// 0 for succeed
// 1 for failed
int  mo_FirmwareUpdateClass_st_system_isreset_hal(void)
{
    MO_DEBUG(("reset_flag = %d\r\n",intorobot_system_param.reset_flag));
    if (1 == intorobot_system_param.reset_flag)
    {
        return 0;
    }
    return 1;

    MO_ERROR(("not support"));
    return 0;
}

// 0 for succeed
// 1 for failed
int mo_FirmwareUpdateClass_st_system_clearreset_hal(void)
{
    intorobot_system_param.reset_flag = 0;
    saveSystemParams( &intorobot_system_param );

    MO_ERROR(("not support"));
    return 0;
}







//=================================================================================================================
//output
/****************************************************************************
*Public Included Files
****************************************************************************/
//=================================================================================================================




