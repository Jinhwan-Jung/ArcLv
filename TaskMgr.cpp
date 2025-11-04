/**
****************************************************************
                            TaskMgr.cpp
    타이머 인터럽트 초기화 (예: Timer1 1ms 주기)
    volatile bool task1msFlag 제공
    ISR 내부에서 flag set → loop()에서 확인 후 실행
****************************************************************
*/
#include "includes.h"

/* 당신이 작성한 매크로/상수들은 그대로 */
#define M_TICK_TIME              10
#define M_TICK_PER_SEC          (1000 / M_TICK_TIME)

#define M_ARC_ACT_TIME_5SEC     (  5 * M_TICK_PER_SEC)
#define M_ARC_ON_TIME_50MS      ( 50 / M_TICK_TIME)
#define M_ARC_ON_TIME_100MS     (150 / M_TICK_TIME)   // 주석대로 150ms 유지

#define M_ARC_FREQ_LV2          1
#define M_ARC_FREQ_LV3          8
#define M_ARC_FREQ_LV4         17
#define M_ARC_FREQ_LV5         30

volatile bool   TaskMgr_Sys1ms_flg = 0;

Ticker          TaskMgr_Ticker;
int             TaskMgr_ArcActTime;

static void TaskMgr_TickTimer(void);
static void TaskMgr_ArcLv_1(int sysTime);
static void TaskMgr_ArcLv_2(int sysTime);
static void TaskMgr_ArcLv_3(int sysTime);
static void TaskMgr_ArcLv_4(int sysTime);
static void TaskMgr_ArcLv_5(int sysTime);

void TaskMgr_Init(void) {
    TaskMgr_Ticker.attach((float)M_TICK_TIME / 1000.0f, TaskMgr_TickTimer);
}

static void TaskMgr_TickTimer(void) {
    TaskMgr_Sys1ms_flg = true;
}

void TaskMgr_ArcGen(void)
{
    static String sysState_Old;

    if (TaskMgr_Sys1ms_flg) {
        TaskMgr_Sys1ms_flg = false;

        if (sysState_Old != sysState) {
            sysState_Old = sysState;

            if (sysState == "START") {
                /* ★ Arc 시작: 애니메이션 일시정지 */
                Disp_FireworkPause(true);

                TaskMgr_ArcActTime = M_ARC_ACT_TIME_5SEC;
                Serial.println("Arc Time Start");
            }
        }

        if (TaskMgr_ArcActTime > 0) {
            switch (arcLevel) {
                case   1: TaskMgr_ArcLv_1(TaskMgr_ArcActTime);  break;
                case   2: TaskMgr_ArcLv_2(TaskMgr_ArcActTime);  break;
                case   3: TaskMgr_ArcLv_3(TaskMgr_ArcActTime);  break;
                case   4: TaskMgr_ArcLv_4(TaskMgr_ArcActTime);  break;
                case   5: TaskMgr_ArcLv_5(TaskMgr_ArcActTime);  break;
                default : TaskMgr_ArcLv_1(TaskMgr_ArcActTime);  break;
            }

            if (--TaskMgr_ArcActTime == 0) {
                sysState = "STOP";
                updated  = true;
                IoCtrl_ArcOff();
                Serial.println("Arc Time Stop");

                /* ★ Arc 종료: 애니메이션 재개 */
                Disp_FireworkPause(false);
                Disp_UpdateFireworkTicker();
            }
        }
    }
}

static void TaskMgr_ArcLv_1(int sysTime)
{ 
    IoCtrl_ArcOff(); 
}


static void TaskMgr_ArcLv_2(int sysTime)
{
    int start = M_ARC_ACT_TIME_5SEC;
    int end   = start - M_ARC_ON_TIME_100MS - M_ARC_ON_TIME_50MS;

    if  ((sysTime <= start) && (sysTime > end)) {
        IoCtrl_ArcOn();
    }
    else {
        IoCtrl_ArcOff();
    }
}


static void TaskMgr_ArcLv_3(int sysTime)
{
    static int  arcSchedule[M_ARC_FREQ_LV3];
    static bool initialized = false;

    if (!initialized && sysTime == M_ARC_ACT_TIME_5SEC) {
        int base = M_ARC_ACT_TIME_5SEC;
        for (int i = 0; i < M_ARC_FREQ_LV3; i++) {
            /* ★ 오타 보정: M_TICK_PER_SEC 사용 */
            arcSchedule[i] = base - random(10, 4 * M_TICK_PER_SEC); // 100ms~4초
        }
        initialized = true;
    }

    bool active = false;
    for (int i = 0; i < M_ARC_FREQ_LV3; i++) {
        if ((sysTime <= arcSchedule[i]) &&
            (sysTime >  arcSchedule[i] - M_ARC_ON_TIME_100MS)) {
            active = true; break;
        }
    }

    if (active) IoCtrl_ArcOn();
    else        IoCtrl_ArcOff();

    if (sysTime == 0) initialized = false;
}

static void TaskMgr_ArcLv_4(int sysTime)
{
    static int  arcSchedule[M_ARC_FREQ_LV4];
    static bool initialized = false;

    if (!initialized && sysTime == M_ARC_ACT_TIME_5SEC) {
        int base = M_ARC_ACT_TIME_5SEC;
        for (int i = 0; i < M_ARC_FREQ_LV4; i++) {
            arcSchedule[i] = base - random(10, 4 * M_TICK_PER_SEC);
        }
        initialized = true;
    }

    bool active = false;
    for (int i = 0; i < M_ARC_FREQ_LV4; i++) {
        if ((sysTime <= arcSchedule[i]) &&
            (sysTime >  arcSchedule[i] - M_ARC_ON_TIME_100MS)) {
            active = true; break;
        }
    }

    if (active) IoCtrl_ArcOn();
    else        IoCtrl_ArcOff();

    if (sysTime == 0) initialized = false;
}

static void TaskMgr_ArcLv_5(int sysTime)
{
    static int  arcSchedule[M_ARC_FREQ_LV5];
    static bool initialized = false;

    if (!initialized && sysTime == M_ARC_ACT_TIME_5SEC) {
        int base = M_ARC_ACT_TIME_5SEC;
        for (int i = 0; i < M_ARC_FREQ_LV5; i++) {
            arcSchedule[i] = base - random(10, (int)(4.4 * M_TICK_PER_SEC));
            if (arcSchedule[i] - M_ARC_ON_TIME_100MS < 0)
                arcSchedule[i] = M_ARC_ON_TIME_100MS;
        }
        initialized = true;
    }

    bool active = false;
    for (int i = 0; i < M_ARC_FREQ_LV5; i++) {
        if ((sysTime <= arcSchedule[i]) &&
            (sysTime > arcSchedule[i] - M_ARC_ON_TIME_100MS)) {
            active = true; break;
        }
    }

    if (active) IoCtrl_ArcOn();
    else        IoCtrl_ArcOff();

    if (sysTime == 0) initialized = false;
}


