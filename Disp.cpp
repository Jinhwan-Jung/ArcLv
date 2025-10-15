/**
****************************************************************
                            Disp.cpp
    TFT LCD 초기화 함수
    설정값 표시 함수 (예: showConfig() / updateStatus())
****************************************************************
*/
#include "includes.h"

#define M_LCD_DEGREE_0   0  // USB 커넥터가 아래 방향
#define M_LCD_DEGREE_90  1  // USB 커넥터가 우측 방향
#define M_LCD_DEGREE_180 2  // USB 커넥터가 위   방향
#define M_LCD_DEGREE_270 3  // USB 커넥터가 좌측 방향

#define M_FIRE_POS_X1 215
#define M_FIRE_POS_X2 240
#define M_FIRE_POS_Y1 60
#define M_FIRE_POS_Y2 100
#define M_FIRE_MARGIN 20    // 폭죽 최대 길이(15px)보다 살짝 크게


Ticker Disp_Firework_Ticker;   // 폭죽 깜빡임 타이머
bool fireworkVisible = false;  // 현재 폭죽 상태 (켜짐/꺼짐)


/**
****************************************************************
    Globaval Variable
****************************************************************
*/
TFT_eSPI tft = TFT_eSPI();


#define TFT_VIVID_ORANGE 0xFBE0  // (255,120,0)

// 레벨 색상표 (아래에서 위로 순서)
uint16_t levelColors[5] = {
    TFT_GREEN,
    TFT_CYAN,
    TFT_YELLOW,
    TFT_VIVID_ORANGE,
    TFT_RED
};


/**
****************************************************************
    Static Fucntion Prototype
****************************************************************
*/
static void Disp_FireworkBlink(void);
static void Disp_DrawFirework(bool on);
static void Disp_ClearFireworkArea(void);

/**
****************************************************************
    Function : Disp_Init
****************************************************************
*/
void Disp_Init(void) {
    tft.init();
    tft.setRotation(M_LCD_DEGREE_270);
    tft.fillScreen(TFT_BLACK);

    Disp_ShowStatus();
    Disp_UpdateFireworkTicker();  // 레벨 상태에 맞춰 시작
}


/**
****************************************************************
    Function : Disp_ShowStatus - 상태 표시 함수
****************************************************************
*/
void Disp_ShowStatus() {
    tft.fillScreen(TFT_BLACK);

    // --- 윗줄: START/STOP ---
    tft.setTextDatum(MC_DATUM);
    tft.setTextFont(4);
    tft.setTextSize(1);
    if (sysState == "START") {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK);
    }
    tft.drawString(sysState, tft.width() / 2, 20);

    // --- Arc / Level 글자 ---
    tft.setTextFont(2);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString("Arc", 35, 70);
    tft.drawString("Level", 35, 100);

    // --- 숫자 표시 ---
    tft.setTextFont(4);
    tft.setTextSize(3);
    tft.setTextColor(levelColors[arcLevel - 1], TFT_BLACK);
    tft.drawString(String(arcLevel), 100, 90);

    // --- 세로 막대 ---
    int barX = 140;
    int barY = 110;
    int barW = 50;
    int barH = 9;
    int gap  = 3;

    for (int i = 0; i < 5; i++) {
        int rectY = barY - i * (barH + gap);
        if (i < arcLevel) {
            tft.fillRect(barX, rectY - barH, barW, barH, levelColors[i]);
        } else {
            tft.drawRect(barX, rectY - barH, barW, barH, TFT_WHITE);
        }
    }
}


/**
****************************************************************
    Function : updateFireworkTicker
****************************************************************
*/
void Disp_UpdateFireworkTicker(void) {
    Disp_Firework_Ticker.detach();  // 기존 주기 반드시 해제

    if ((sysState == "STOP") || (arcLevel <= 1)) {
        Disp_DrawFirework(false);  // 화면 클리어
        return;
    }

    float interval = 0.5;  // 기본값
    switch (arcLevel) {
        case 2: interval = 0.8;  break;   // 느리게
        case 3: interval = 0.5;  break;   // 보통
        case 4: interval = 0.3;  break;   // 조금 빠르게
        case 5: interval = 0.15; break;   // 매우 빠르게
    }

    Disp_Firework_Ticker.attach(interval, Disp_FireworkBlink);
}

/**
****************************************************************
    Function : fireworkBlink
****************************************************************
*/
static void Disp_FireworkBlink(void) {
    fireworkVisible = !fireworkVisible;  // 상태 토글
    Disp_DrawFirework(fireworkVisible);
}


/**
****************************************************************
    Function : Disp_DrawFirework
****************************************************************
*/
static void Disp_DrawFirework(bool on) {
    // Stop이거나 레벨 1 → 무조건 화면 클리어
    if ((sysState == "STOP") || (arcLevel == 1)) {
        // 폭죽 전체 영역 클리어
        Disp_ClearFireworkArea();
    } else {
        if (on) {
            // 🔥 폭죽 그리기
            int cx = random(M_FIRE_POS_X1, M_FIRE_POS_X2);
            int cy = random(M_FIRE_POS_Y1, M_FIRE_POS_Y2);
            uint16_t colors[5] = { TFT_WHITE, TFT_YELLOW, TFT_RED, TFT_CYAN, TFT_VIVID_ORANGE };

            int rays = random(6, 10);
            for (int i = 0; i < rays; i++) {
                float angle = (360.0 / rays) * i + random(-10, 10);
                float rad = angle * 3.14159 / 180.0;
                int len = random(8, 15);
                int x2 = cx + cos(rad) * len;
                int y2 = cy + sin(rad) * len;
                uint16_t c = colors[random(0, 5)];

                tft.drawLine(cx, cy, x2, y2, c);
                tft.drawLine(cx + 1, cy, x2 + 1, y2, c);
            }
        } else {
            // 🔄 지우기 (검정으로 덮음)
            Disp_ClearFireworkArea();
        }
    }
}


/**
****************************************************************
    Function : Disp_ClearFireworkArea
****************************************************************
*/
static void Disp_ClearFireworkArea(void) {
    tft.fillRect(
        M_FIRE_POS_X1 - M_FIRE_MARGIN,
        M_FIRE_POS_Y1 - M_FIRE_MARGIN,
        (M_FIRE_POS_X2 - M_FIRE_POS_X1) + (M_FIRE_MARGIN * 2),
        (M_FIRE_POS_Y2 - M_FIRE_POS_Y1) + (M_FIRE_MARGIN * 2),
        TFT_BLACK);
}