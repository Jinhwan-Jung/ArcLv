# ArcLv
- MCU
  - ESP32 TFT : LilyGo T-Display

- IDUINO 환경 설정
  - 기본설정에서
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

  - 보드 매니저에서
    esp32 by Espressif Systems 설치

  - 라이브러리 매니저에서
    tft_espi by Bodmer 설치

  - C:\Users\jhjun\OneDrive\문서\Arduino\libraries\TFT_eSPI 폴더에서
    User_Setup_Select.h 열기
      // #include <User_Setup.h> 는 주석 처리
      #include <User_Setups/Setup25_TTGO_T_Display.h> 는 주석 해제
