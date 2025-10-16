/**
****************************************************************
                            TaskMgr.cpp
    타이머 인터럽트 초기화 (예: Timer1 1ms 주기)
    volatile bool task1msFlag 제공
    ISR 내부에서 flag set → loop()에서 확인 후 실행
****************************************************************
*/
#include "includes.h"


#define M_ARC_ACT_TIME_5SEC     (5 * 1000)  // 1ms 주기로 설정
#define M_ARC_START_DELAY       (500)       // 패턴 시작 후 500ms 대기
#define M_ARC_ON_TIME_50MS      (150)
#define M_ARC_ON_TIME_100MS     (300)



/**
****************************************************************
    Globaval Variable
****************************************************************
*/
volatile bool   TaskMgr_Sys1ms_flg = 0;

Ticker          TaskMgr_Ticker;
int             TaskMgr_ArcActTime;


/**
****************************************************************
    Local Function 
****************************************************************
*/
static void TaskMgr_ArcLv_1(int sysTime);
static void TaskMgr_ArcLv_2(int sysTime);
static void TaskMgr_ArcLv_3(int sysTime);
static void TaskMgr_ArcLv_4(int sysTime);
static void TaskMgr_ArcLv_5(int sysTime);


/**
****************************************************************
    Function : TaskMgr_Init
****************************************************************
*/
void TaskMgr_Init(void) {
  TaskMgr_Ticker.attach(0.001, TaskMgr_1ms);
}

/**
****************************************************************
    Function : TaskMgr_1ms - 주기 실행 함수
    SW 기반이므로 정밀하지 않다. 
****************************************************************
*/
void TaskMgr_1ms(void) {
  TaskMgr_Sys1ms_flg = true;
}


/**
****************************************************************
    Function : TaskMgr_ArcGen
    1ms마다 호출되어야 한다.
****************************************************************
*/
void TaskMgr_ArcGen(void)
{
    static String sysState_Old;
    
    if (TaskMgr_Sys1ms_flg == true) {
        TaskMgr_Sys1ms_flg = false;

        // 아크 발생 시간 시작 설정
        if (sysState_Old != sysState) {
            sysState_Old = sysState;

            if (sysState == "START") {
                TaskMgr_ArcActTime = M_ARC_ACT_TIME_5SEC;
                Serial.println("Arc Time Start");
            }
        }

        // 레벨 별 아크 발생 시나리오 
        if (TaskMgr_ArcActTime > 0) {
            switch (arcLevel) {
                case   1: TaskMgr_ArcLv_1(TaskMgr_ArcActTime);  break;
                case   2: TaskMgr_ArcLv_2(TaskMgr_ArcActTime);  break;
                case   3: TaskMgr_ArcLv_3(TaskMgr_ArcActTime);  break;
                case   4: TaskMgr_ArcLv_4(TaskMgr_ArcActTime);  break;
                case   5: TaskMgr_ArcLv_5(TaskMgr_ArcActTime);  break;
                default : TaskMgr_ArcLv_1(TaskMgr_ArcActTime);  break;
            }
        
            // 아크 발생 시간 종료 후 동작 정의
            if (--TaskMgr_ArcActTime == 0) {
                sysState = "STOP";
                updated  = true;
                IoCtrl_ArcOff();
                Serial.println("Arc Time Stop");
            }
        }
    }
}


/**
****************************************************************
    Function : TaskMgr_ArcLv_1
****************************************************************
*/
static void TaskMgr_ArcLv_1(int sysTime)
{
    IoCtrl_ArcOff();
}

/**
****************************************************************
    Function : TaskMgr_ArcLv_2 
    - 500ms 뒤 1회 50ms 아크
****************************************************************
*/
static void TaskMgr_ArcLv_2(int sysTime)
{
    int start = M_ARC_ACT_TIME_5SEC - M_ARC_START_DELAY;        // 4500
    int end   = start - M_ARC_ON_TIME_50MS;                     // 4450

    if  ((sysTime <= start) && (sysTime > end)){
        IoCtrl_ArcOn();
    }
    else {
        IoCtrl_ArcOff();
    }

#if 0  
    if (  (sysTime < (M_ARC_ACT_TIME_5SEC-1000)) 
       && (sysTime > (M_ARC_ACT_TIME_5SEC-1500)) ) {
        IoCtrl_ArcOn();
    }
    else {
        IoCtrl_ArcOff();
    }
#endif    
}



/**
****************************************************************
    Function : TaskMgr_ArcLv_3 
     - 랜덤 간격, 50ms 아크 × 5회
****************************************************************
*/
static void TaskMgr_ArcLv_3(int sysTime)
{
    static int  arcSchedule[5];
    static bool initialized = false;

    if (!initialized && sysTime == M_ARC_ACT_TIME_5SEC) {
        int base = M_ARC_ACT_TIME_5SEC - M_ARC_START_DELAY; // 4500
        for (int i = 0; i < 5; i++) {
            arcSchedule[i] = base - random(100, 4000);      // 100~4000ms 내
        }
        initialized = true;
    }

    bool active = false;
    for (int i = 0; i < 5; i++) {
        if ((sysTime <= arcSchedule[i]) &&
            (sysTime >  arcSchedule[i] - M_ARC_ON_TIME_50MS)) {
            active = true;
            break;
        }
    }

    if (active) IoCtrl_ArcOn();
    else        IoCtrl_ArcOff();

    if (sysTime == 0) {
        initialized = false;                              // 다음 사이클을 위해 초기화
    }
}


/**
****************************************************************
    Function : TaskMgr_ArcLv_4
     - 랜덤 간격, 100ms 아크 × 10회
    
****************************************************************
*/
static void TaskMgr_ArcLv_4(int sysTime)
{
    static int  arcSchedule[10];
    static bool initialized = false;

    if (!initialized && sysTime == M_ARC_ACT_TIME_5SEC) {
        int base = M_ARC_ACT_TIME_5SEC - M_ARC_START_DELAY;
        for (int i = 0; i < 10; i++) {
            arcSchedule[i] = base - random(100, 4000);
        }
        initialized = true;
    }

    bool active = false;
    for (int i = 0; i < 10; i++) {
        if ((sysTime <= arcSchedule[i]) &&
            (sysTime >  arcSchedule[i] - M_ARC_ON_TIME_100MS)) {
            active = true;
            break;
        }
    }

    if (active) IoCtrl_ArcOn();
    else        IoCtrl_ArcOff();

    if (sysTime == 0) {
        initialized = false;
    }
}


/**
****************************************************************
    Function : TaskMgr_ArcLv_5
     - 덤 간격, 100ms 아크 × 30회 (5초 초과 방지 버전)
****************************************************************
*/
static void TaskMgr_ArcLv_5(int sysTime)
{
    static int  arcSchedule[30];
    static bool initialized = false;

    if (!initialized && sysTime == M_ARC_ACT_TIME_5SEC) {
        int base = M_ARC_ACT_TIME_5SEC - M_ARC_START_DELAY;   // 4500
        for (int i = 0; i < 30; i++) {
            arcSchedule[i] = base - random(100, 4400);        // ✅ 4400으로 제한
            // 경계 보정 (음수 방지)
            if (arcSchedule[i] - M_ARC_ON_TIME_100MS < 0)
                arcSchedule[i] = M_ARC_ON_TIME_100MS;
        }
        initialized = true;
    }

    bool active = false;
    for (int i = 0; i < 30; i++) {
        if ((sysTime <= arcSchedule[i]) &&
            (sysTime > arcSchedule[i] - M_ARC_ON_TIME_100MS)) {
            active = true;
            break;
        }
    }

    if (active) IoCtrl_ArcOn();
    else        IoCtrl_ArcOff();

    if (sysTime == 0) {
        initialized = false;
    }
}

