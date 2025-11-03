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
bool    updated  = false;       // 상태 변화 여부 플래그

bool    btConnected = false;    // 🔵 블루투스 연결 상태

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
//    bool updated = false;                   // 상태 변화 여부 플래그
    
    if (SerialBT.available()) {
        char cmd = SerialBT.read();
        //Serial.write(cmd);

        if (cmd == '\r' || cmd == '\n') 
            return;

        switch (cmd) {
            case 't': if (sysState != "STOP") { sysState = "STOP";  updated = true; } break;

            case 's': if (sysState == "STOP") { sysState = "START"; updated = true; } break;            
            case '1': if (sysState == "STOP") { arcLevel = 1;       updated = true; } break;
            case '2': if (sysState == "STOP") { arcLevel = 2;       updated = true; } break;
            case '3': if (sysState == "STOP") { arcLevel = 3;       updated = true; } break;
            case '4': if (sysState == "STOP") { arcLevel = 4;       updated = true; } break;
            case '5': if (sysState == "STOP") { arcLevel = 5;       updated = true; } break;
            default :                                                                 break;
        }
    }
        
    if (updated) {
        updated = false;
        Disp_ShowStatus();
        Disp_UpdateFireworkTicker();   // ⭐ 상태 바뀔 때마다 다시 세팅
    }

    // 🔵 연결 상태 갱신 (매 주기 확인)
    bool currentConn = SerialBT.hasClient();
    if (currentConn != btConnected) {
        btConnected = currentConn;
        Disp_ShowStatus();            // 아이콘 갱신
    }    
}

