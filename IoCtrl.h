/**
****************************************************************
                            IoCtrl.h
    블루투스 초기화
    수신된 명령 읽기 (getCommand() 반환)
    필요시 콜백 함수 등록 가능
****************************************************************
*/
#ifndef __IO_CTRL_h
#define __IO_CTRL_h

extern void IoCtrl_Init(void);
extern void IoCtrl_SwToPin(void);
extern void IoCtrl_ArcOn(void);
extern void IoCtrl_ArcOff(void);

#endif
