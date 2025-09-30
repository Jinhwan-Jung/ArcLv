/**
****************************************************************
                            BT.h
    블루투스 초기화
    수신된 명령 읽기 (getCommand() 반환)
    필요시 콜백 함수 등록 가능
****************************************************************
*/
#ifndef __BT_h
#define __BT_h


extern String  sysState;
extern int     arcLevel;

extern void BT_Init(void);
extern void BT_Task(void);


#endif
