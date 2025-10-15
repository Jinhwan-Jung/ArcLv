/**
****************************************************************
                            IoCtrl.cpp
    IO 핀 초기화
    명령에 따라 핀 ON/OFF
    task1ms() 안에서 실행될 핵심 함수
****************************************************************
*/
#include "includes.h"


#define M_GPIO_SW_1         0   // 보드 자체 스위치
#define M_GPIO_SW_2         35  // 보드 자체 스위치
#define M_GPIO_ARC_CTRL     27  // GPIO27 아크 출력 제어

/**
****************************************************************
    Function : IoCtrl_Init
****************************************************************
*/
void IoCtrl_Init(void)
{
    /* 보드 자체 스위치를 위한 설정 */
    pinMode     (M_GPIO_SW_1, INPUT_PULLUP);
    
    /* 아크 발생기를 위한 설정 */
    pinMode     (M_GPIO_ARC_CTRL, OUTPUT);  // GPIO37을 출력으로 설정
    digitalWrite(M_GPIO_ARC_CTRL, LOW);     // 초기 상태: OFF
}

/**
****************************************************************
    Function : IoCtrl_SwToPin
****************************************************************
*/
void IoCtrl_SwToPin(void)
{
    int swStat;

    if (sysState == "STOP") {
        swStat = digitalRead(M_GPIO_SW_1);

        if (swStat == LOW) {
            digitalWrite(M_GPIO_ARC_CTRL, HIGH);
        }
        else {
            digitalWrite(M_GPIO_ARC_CTRL, LOW); 
        }
    }
}


/**
****************************************************************
    Function : IoCtrl_ArcOn
****************************************************************
*/
void IoCtrl_ArcOn(void)
{
    digitalWrite(M_GPIO_ARC_CTRL, HIGH);
}

/**
****************************************************************
    Function : IoCtrl_ArcOff
****************************************************************
*/
void IoCtrl_ArcOff(void)
{
    digitalWrite(M_GPIO_ARC_CTRL, LOW);
}

