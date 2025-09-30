/**
****************************************************************
                            BT.cpp
    블루투스 초기화
    수신된 명령 읽기 (getCommand() 반환)
    필요시 콜백 함수 등록 가능
****************************************************************
*/
#include "includes.h"

/**
****************************************************************
    Globaval Variable
****************************************************************
*/
String device_name = "ESP32-BT-Slave";

BluetoothSerial SerialBT;

// 상태 변수
String  sysState = "STOP";      // START/STOP 상태
int     arcLevel = 1;           // 1~5 레벨


/**
****************************************************************
    Function : BT_Init
****************************************************************
*/
void BT_Init(void)
{
    Serial.begin (115200);
    Serial.printf("The device \"%s\" is started.\n", device_name.c_str());

    SerialBT.begin(device_name);
}


/**
****************************************************************
    Function : BT_Task
****************************************************************
*/
void BT_Task(void)
{
    bool updated = false;                   // 상태 변화 여부 플래그
    
    if (SerialBT.available()) {
        char cmd = SerialBT.read();
        Serial.write(cmd);

        if (cmd == '\r' || cmd == '\n') 
            return;

        switch (cmd) {
            case 's': sysState = "START"; updated = true; break;
            case 't': sysState = "STOP";  updated = true; break;
            case '1': arcLevel = 1;       updated = true; break;
            case '2': arcLevel = 2;       updated = true; break;
            case '3': arcLevel = 3;       updated = true; break;
            case '4': arcLevel = 4;       updated = true; break;
            case '5': arcLevel = 5;       updated = true; break;
            default :                                     break;
        }
        
        if (updated) {
            Disp_ShowStatus();
            Disp_UpdateFireworkTicker();   // ⭐ 상태 바뀔 때마다 다시 세팅
        }
    }
}
