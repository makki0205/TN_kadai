//******************************************************************************
//    練習01_RTOS Task間通信 DataQueue Sample_Program               : by  Sumida
//******************************************************************************

//***** Include Header Files ***************************************************
#include "kernel_cfg.h"    // Toppers Kernel Config Header
#include "hal.h"           // Toppers Header
#include "hal_extention.h" // Toppers HAL_Extention Header
DEV dev_table[2];
int drv_api_led(int task_id, int dev_id, int req_cd, int req_ope, int blink_timer, int blink_cycle)
{
    D_PKT dPkt;
    U_PKT uPkt;

    // OPENしているかチェック
    if (req_cd == CTL && req_cd == CLOSE)
    {
        // task_idが違うか開放状態ならエラー
        if (dev_table[dev_id].task_id != task_id && dev_table[dev_id].status == FREE)
        {
            return INV;
        }
    }
    // status確認はメールで送らなくてもできるのですぐに返す
    if (req_cd == CTL && req_ope == LED_STAT)
    {
        if (dev_table[dev_id].status == IN_WAIT)
        {
            return IN_WAIT;
        }
        return FREE;
    }
    // pktを生成して送る
    dPkt.task_id = task_id;
    dPkt.dev_id = dev_id;
    dPkt.req_cd = req_cd;
    dPkt.req_ope = req_ope;
    dPkt.blink_timer = blink_timer;
    dPkt.blink_cycle = blink_cycle;
    DsendMail(dPkt);
    UReceiveMail(task_id, dev_id, *uPkt);
    return uPkt.res;
}

void drv_main(intptr_t exinf)
{
    D_PKT dPkt;
    while (true)
    {
        // 受け取る
        DReceiveMail(*dPkt);

        switch (dPkt.req_cd)
        {
        case OPEN:
            if (dev_table[dPkt.dev_id].status == free)
            {
                // 確保
                dev_table[dPkt.dev_id].status = IN_USE;
                dev_table[dPkt.dev_id].task_id = dPkt.task_id;
                sendMail(dPkt.task_id, dPkt.dev_id, OK);
            }
            sendMail(dPkt.task_id, dPkt.dev_id, IN_USE);
            break;
        case CTL:
            led_ctl(dPkt);
            break;
        case CLOSE:
            dev_table[dPkt.dev_id].status = FREE;
            sendMail(dPkt.task_id, dPkt.dev_id, OK);
            break;
        default:
            break;
        }
    }
}

void led_ctl(D_PKT pkt)
{

    switch (pkt.req_ope)
    {
    case LED_ON:
        write_led(pkt.dev_id, LED_ON);
        break;
    case LED_OFF:
        write_led(pkt.dev_id, LED_OFF);
        break;
    case LED_BLINK:
        dev_table[pkt.dev_id].blink_timer = pkt.blink_timer;
        dev_table[pkt.dev_id].blink_cycle = pkt.blink_cycle;
        dev_table[pkt.dev_id].status = IN_WAIT;

        // TODO akt_tak blinkのタスク起動
        break;
    default:
        break;
    }
}
