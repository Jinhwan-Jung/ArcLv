/**
****************************************************************
                            TaskMgr.h
    타이머 인터럽트 초기화 (예: Timer1 1ms 주기)
    volatile bool task1msFlag 제공
    ISR 내부에서 flag set → loop()에서 확인 후 실행
****************************************************************
*/
#ifndef __TASK_MGR_h
#define __TASK_MGR_h


extern volatile bool task1msFlag ;      // 1ms마다 세트되는 플래그


void TaskMgr_Init(void);
void TaskMgr_1ms(void);

#endif
