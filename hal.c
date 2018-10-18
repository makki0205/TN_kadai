//******************************************************************************
//    練習01_RTOS Task間通信 DataQueue Sample_Program               : by  Sumida
//******************************************************************************

//***** Include Header Files ***************************************************
#include "kernel_cfg.h"    // Toppers Kernel Config Header
#include "hal.h"           // Toppers Header
#include "hal_extention.h" // Toppers HAL_Extention Header
LED led_table[2];
int drv_api_led(int task_id, int dev_id, int req_cd, int req_ope, int blink_timer, int blink_cycle)
{

    if (req_cd == CTL && req_ope == LED_STAT)
    {
    }
}