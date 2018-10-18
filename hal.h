/*
 *  ターゲット依存の定義
 */
#include "target_test.h"
/*
 *  標準Toppers_Header (Moved to here from User_Code) 角田
 */
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
//#include "kernel_cfg.h"

/*
 *  各タスクの優先度の定義
 */
#define MAIN_PRIORITY 5 /* メインタスクの優先度 */
						/* HIGH_PRIORITYより高くすること */
#define HIGH_PRIORITY 9 /* 並行実行されるタスクの優先度 */
#define MID_PRIORITY 10
#define LOW_PRIORITY 11

/*
 *  ターゲットに依存する可能性のある定数の定義
 */
#ifndef TASK_PORTID
#define TASK_PORTID 1 /* 代替シリアルポートID */
#endif				  /* TASK_PORTID */

#ifndef STACK_SIZE
#define STACK_SIZE 4096 /* タスクのスタックサイズ */
#endif					/* STACK_SIZE */

#ifndef LOOP_REF
#define LOOP_REF ULONG_C(1000000) /* 速度計測用のループ回数 */
#endif

//mpfmanage()
#define GET 1
#define RLS 2
#define CLN 3

// dev_id
#define LED1 1
#define LED2 2

// req_cd
#define OPEN 1
#define CTL 2
#define CLOSE 2

// req_ope
#define LED_ON 1
#define LED_ON 2
#define LED_BLINK 3
#define LED_STAT 4

// res
#define OK 0
#define INV 1 // 引数エラー
#define IN_USE 2
#define IN_WAIT 3
#define FREE 0

typedef struct
{
	T_MSG pk_msg;
	int task_id;
	int dev_id;
	int req_cd;
	int req_ope;
	int blink_timer;
	int blink_cycle;
} D_PKT;

typedef struct
{
	T_MSG pk_msg;
	int task_id;
	int dev_id;
	int res;
} U_PKT;

typedef struct
{
	int status;
	int task_id;
	int dev_id;
	int blink_timer;
	int blink_cycle;
} LED;

/*
 *  タスクのプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY
//*********************************************************************
// mail関係
void DsendMail(D_PKT pkt);
void USendMail(U_PKT pkt);
void DSeceiveMail(D_PKT *pkt);
void USeceiveMail(int task_id, int dev_id, U_PKT *pkt);
void mpfmanage(int ope, MBX_PKT *packet);

//
int drv_api_led(int task_id, int dev_id, int req_cd, int req_ope, int blink_timer, int blink_cycle);
void drv_main(intptr_t exinf);
void blink_led(int no, int blink_timer, int blink_cycle);
void write_led(int no, int status);
//*********************************************************************
#endif /* TOPPERS_MACRO_ONLY */
