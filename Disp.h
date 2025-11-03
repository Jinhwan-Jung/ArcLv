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

/* ★ 추가: 안전한 렌더링을 위한 루프측 처리 및 일시정지 제어 */
extern void Disp_Task(void);
extern void Disp_FireworkPause(bool on);

#endif