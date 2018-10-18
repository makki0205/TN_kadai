#include "kernel_cfg.h"    // Toppers Kernel Config Header
#include "hal.h"           // Toppers Header
#include "hal_extention.h" // Toppers HAL_Extention Header

void mpfmanage(int ope, MBX_PKT *packet)
{
    static MBX_PKT *addr[10] = {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };
    int i;

    if (ope == GET)
    {
        for (i = 0; addr[i] != 0; i++)
        {
        }
        addr[i] = packet;
    }
    else if (ope == RLS)
    {
        for (i = 0; addr[i] != packet; i++)
        {
        }
        addr[i] = 0;
    }
    else if (ope == CLN)
    {
        for (i = 0; i < 10; i++)
        {
            if (addr[i] != 0)
            {
                rel_mpf(MPF, addr[i]);
            }
        }
    }
}
void sendMail(int fromID, int toID, char *msg)
{
    MBX_PKT *packet1;

    get_mpf(MPF, (void **)&packet1); // MBX Packet獲得
    mpfmanage(GET, packet1);
    packet1->fromID = fromID;
    packet1->toID = toID;
    strcpy(packet1->message, msg);
    snd_mbx(MBX, (T_MSG *)packet1);
}

// void receiveMail(int toID, int *pFromID, char *msg)
// {
//     T_RMBX mbx_status;
//     MBX_PKT *packet;
//     if (pFromID == 0)
//     {
//         syslog_0(LOG_NOTICE, "receiveMail() is Err pFromID is null");
//         return;
//     }
//     if (msg == 0)
//     {
//         syslog_0(LOG_NOTICE, "receiveMail() is Err msg is null");
//         return;
//     }

//     for (;;)
//     {
//         ref_mbx(MBX, &mbx_status);
//         if (mbx_status.pk_msg != NULL)
//         {
//             packet = (MBX_PKT *)mbx_status.pk_msg;
//             if (packet->toID == toID)
//             {
//                 rcv_mbx(MBX, (T_MSG **)&packet);
//                 *pFromID = packet->fromID;
//                 strcpy(msg, packet->message);
//                 rel_mpf(MPF, packet);
//                 mpfmanage(RLS, packet);
//                 break;
//             }
//         }
//         dly_tsk(3);
//     }
// }