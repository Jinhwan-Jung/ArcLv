/**
****************************************************************
                            Disp.h
    TFT LCD 초기화 함수
    설정값 표시 함수 (예: showConfig() / updateStatus())
****************************************************************
*/
#ifndef __DISP_h
#define __DISP_h

extern void Disp_Init(void);
extern void Disp_ShowStatus();
extern void Disp_DrawArcEffect(void);

extern void Disp_UpdateFireworkTicker(void);

#endif