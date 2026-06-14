#define _CRT_SECURE_NO_WARNINGS           // 구동시 예외처리

#pragma once
#include <stdio.h> 
#include <windows.h> 
#include <conio.h> 
#include <time.h>
#include <locale.h>
#include <mmsystem.h>                    // 사운드 재생에 필요한 라이브러리 링크
#include <string.h> 

#pragma execution_character_set("utf-8") // 하행 문자열들을 UTF-8 형식으로 인코딩후 저장하라는 전처리 명령
#pragma comment(lib, "winmm.lib")        // MSVC 컴파일러용 라이브러리 링크



//Edited By 현진 From 06-14 AM 04:24

// 월드 및 스크린 크기 정의
#define SCR_W 80  
#define SCR_H 28  
#define WORLD_W 100
#define WORLD_H 100
#define CONSOLE_H 30 // 콘솔 크기

// 색상 지정 상수
#define BG_COLOR_BLACK   40
#define BG_COLOR_RED     41
#define BG_COLOR_BLUE    44
#define BG_COLOR_YELLOW  43
#define BG_COLOR_BrRED   101
#define BG_COLOR_BrGREEN 102
#define FONT_COLOR_BrGREEN 32
#define FONT_COLOR_RED 31
#define FONT_COLOR_WHITE 37
#define FONT_COLOR_BLACK 30
#define COLOR_RESET      "\x1b[0m"

// 공통 전역 변수 구조체 선언
typedef struct {
    int day;
    int money;
    int satisfaction;
    int opend;
    int inventory[7];
} GameState;

typedef struct {
    char dialogue[200];
    char answer[50];
    char precise_success[100];
    char precise_fail[100];
} Demand;

typedef struct {
    char chara[256];
    char fail_art_file[512];
    char success_chara[256];
    Demand demands[3];
    int num_demands;
} CustomerProfile;

typedef struct {
    char name[50]; // 약 이름
    int sell_price;     // 약 가격
    int buy_price;
} Medicine;


// 구조체 선언들 밑에, 함수들 시작하기 전 빈 공간에 선언해!
Medicine stock[7] = {
    {"두통약", 3000, 1500},
    {"소화제", 2500, 1500},
    {"회복포션", 1500, 500},
    {"해독제", 10000, 5000},
    {"해열제", 4000, 2000},
    {"수면제", 4500, 2000},
    {"감기약", 8000, 4000}
};



//커서 지우기
void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE; // 커서를 투명하게 만들어버림!
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}


// 커서 위치 이동 함수
void Move_Cursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
}


void set_color(int code) {
    printf("\x1b[%dm", code);
}


// 자동개행
// 시작 좌표(startX, startY)와 텍스트, 그리고 텍스트가 넘어갈 최대 너비(max_width)를 받음
void print_dialogue_wrapped(int startX, int startY, const char* text, int max_width) {
    int currentX = startX;
    int currentY = startY;
    int i = 0;

    Move_Cursor(currentX, currentY);
    set_color(FONT_COLOR_BLACK); // 글자는 검은색
    printf("\033[107m");         // 배경은 흰색 (말풍선 효과 시작!)

    while (text[i] != '\0') {
        // 1. 한글(2바이트)인지 확인 (C언어 콘솔에서 한글은 음수 값을 가짐)
        if (text[i] < 0) {
            // 출력하면 지정한 너비를 뚫고 나갈 것 같으면 미리 개행 처리!
            if (currentX - startX + 2 > max_width) {
                printf("\033[0m"); // 이전 줄의 흰색 배경을 잠깐 끄고
                currentY++;
                currentX = startX;
                Move_Cursor(currentX, currentY);
                set_color(FONT_COLOR_BLACK);
                printf("\033[107m"); // 다음 줄로 넘어와서 흰색 배경 다시 켬!
            }
            printf("%c%c", text[i], text[i + 1]);
            i += 2;
            currentX += 2;
        }
        // 2. 영어, 숫자, 띄어쓰기, 기호 (1바이트) 처리
        else {
            if (currentX - startX + 1 > max_width) {
                printf("\033[0m");
                currentY++;
                currentX = startX;
                Move_Cursor(currentX, currentY);
                set_color(FONT_COLOR_BLACK);
                printf("\033[107m");
            }
            printf("%c", text[i]);
            i += 1;
            currentX += 1;
        }
    }
    printf("\033[0m"); // 대사 출력이 모두 끝나면 색상 완벽하게 초기화
}



void Force_English_Mode() {
    // 게임 실행 중에 윈도우의 한글 입력기 창고(dll)를 강제로 열어버림!
    HMODULE hImm = LoadLibrary(TEXT("imm32.dll"));
    if (hImm != NULL) {
        typedef HIMC(WINAPI* GETCONTEXT)(HWND);
        typedef BOOL(WINAPI* RELEASECONTEXT)(HWND, HIMC);
        typedef BOOL(WINAPI* SETSTATUS)(HIMC, DWORD, DWORD);

        GETCONTEXT getCtx = (GETCONTEXT)GetProcAddress(hImm, "ImmGetContext");
        RELEASECONTEXT relCtx = (RELEASECONTEXT)GetProcAddress(hImm, "ImmReleaseContext");
        SETSTATUS setStat = (SETSTATUS)GetProcAddress(hImm, "ImmSetConversionStatus");

        if (getCtx && relCtx && setStat) {
            HWND hwnd = GetConsoleWindow();
            HIMC himc = getCtx(hwnd);
            // 0 = IME_CMODE_ALPHANUMERIC (완벽한 순수 영문 모드)
            setStat(himc, 0, 0);
            relCtx(hwnd, himc);
        }
        FreeLibrary(hImm); // 일 다 했으니 창고 문 닫기
    }
}

// 헬레나님의 절대 무적 한글 모드 강제 변환기
void Force_Korean_Mode() {
    HMODULE hImm = LoadLibrary(TEXT("imm32.dll"));
    if (hImm != NULL) {
        typedef HIMC(WINAPI* GETCONTEXT)(HWND);
        typedef BOOL(WINAPI* RELEASECONTEXT)(HWND, HIMC);
        typedef BOOL(WINAPI* SETSTATUS)(HIMC, DWORD, DWORD);

        GETCONTEXT getCtx = (GETCONTEXT)GetProcAddress(hImm, "ImmGetContext");
        RELEASECONTEXT relCtx = (RELEASECONTEXT)GetProcAddress(hImm, "ImmReleaseContext");
        SETSTATUS setStat = (SETSTATUS)GetProcAddress(hImm, "ImmSetConversionStatus");

        if (getCtx && relCtx && setStat) {
            HWND hwnd = GetConsoleWindow();
            HIMC himc = getCtx(hwnd);
            // 1 = IME_CMODE_NATIVE (완벽한 순수 한글 모드)
            setStat(himc, 1, 0);
            relCtx(hwnd, himc);
        }
        FreeLibrary(hImm);
    }
}




// 아스키 아트 그리기 함수
void draw_ascii_art(const char* filepath, int startX, int startY) {
    FILE* fp = fopen(filepath, "r");
    char line[4096];

    if (fp) {
        int yOffset = 0;
        while (fgets(line, sizeof(line), fp) != NULL) {
            line[strcspn(line, "\n")] = 0;
            Move_Cursor(startX, startY + yOffset);
            printf("%s", line);
            yOffset++;
        }
        fclose(fp);
    }
    else {
        Move_Cursor(startX, startY);
        printf("[!] 파일 로드 실패: %s", filepath);
    }
}

// 픽셀 그리기 함수
void draw_pixel(int x, int y, int r, int g, int b) {
    if (x < 0 || x >= SCR_W || y < 0 || y >= SCR_H) return;
    printf("\033[%d;%dH\033[48;2;%d;%d;%dm  \033[0m", y + 1, (x * 2) + 1, r, g, b);
}

// 폰트 색상 변경 함수


// 스프라이트 데이터 (상수)
const char* PHARMACY[] = {
    "GGGGGGGGG", "GGGGRGGGG", "GGGGRGGGG", "GGRRRRRGG",
    "GGGGRGGGG", "GGGGRGGGG", "GGGGGGGGG", "GGGGGGGGG"
};

const char* HOUSE[] = {
    ".....R.....", "....RRR....", "...RRRRR...", "..RRRRRRR..", ".RRRRRRRRR.",
    ".YYYYYYYYY.", ".YYWYYYWYY.", ".YYYYDYYYY.", ".YYYYYYYYY."
};

const char* SKY[] = {
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWCCCWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWCCCCCWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWCCCCWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWCCCCCCCCWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWCCCWWWWWWWWWW",
    "WWWWWWWWWCCWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWCCCCCCCWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWCCCWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
};


const char* SKY_NIGHT[] = {
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMLLLMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMLLLLLMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMLLLLMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMLLLLLLLLLMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMLLLMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMLLLLLLLMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    "MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM", "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
};

const char* ROOM[] = {
    "HHHQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQHHH", "HHHHQQQQQQQQQQQCCCCCCCCCCQQQQQQQQQQQHHHH",
    "HHHHHQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQHHHHH", "HHHHHHQQQQQQQQQQQQQQQQQQQQQQQQQQQQHHHHHH",
    "HHHHHHHQQQQQQQQQQQQQQQQQQQQQQQQQQHHHHHHH", "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH", "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJWWWWWWWWJJJJJJJJJJJJJJHHHHHHH", "HHHHHHHJJJJWWWWWWWWJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJWWWWWWWWJJJJJJJJJJJJJJHHHHHHH", "HHHHHHHJJJDDDDDDDDDDJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH", "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH", "HHHHHHHNNNNNNJJJJJJJJJJJJJJDDDDDDHHHHHHH",
    "HHHHHHNNNNNNNJJJJJJJJJJJJJJDDDDDDHHHHHHH", "HHHHHNCCCCCCNJJJJJJJJJJJJJJDDCCCDHHHHHHH",
    "HHHHHNJJJJJJNJJJJJJJJJJJJJJDRRRRRRHHHHHH", "HHHHHNBBBBBBNBBBBBBBBBBBBBBDDRRRRRRHHHHH",
    "HHHHHBBBBBBBBBBBBBBBBBBBBBBDBDRRRRRRHHHH", "HHHHBBBBBBBBBBBBBBBBBBBBBBBBBBDRRRRRRHHH",
    "HHHBBBBBBBBBBBBBBBBBBBBBBBBBBBBDRRRRRRHH", "HHBBBBBBBBBBBBBBBBBBBBBBBBBBBBBDDDDDDDDH",
    "HBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBDBBBBBBDH"
};

const char* ROOM_NIGHT[] = {
    "HHHQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQHHH", "HHHHQQQQQQQQQQQCCCCCCCCCCQQQQQQQQQQQHHHH",
    "HHHHHQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQHHHHH", "HHHHHHQQQQQQQQQQQQQQQQQQQQQQQQQQQQHHHHHH",
    "HHHHHHHQQQQQQQQQQQQQQQQQQQQQQQQQQHHHHHHH", "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH", "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJMMMMMMMMJJJJJJJJJJJJJJHHHHHHH", "HHHHHHHJJJJMMMMMMMMJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJMMMMMMMMJJJJJJJJJJJJJJHHHHHHH", "HHHHHHHJJJDDDDDDDDDDJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH", "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH", "HHHHHHHNNNNNNJJJJJJJJJJJJJJDDDDDDHHHHHHH",
    "HHHHHHNNNNNNNJJJJJJJJJJJJJJDDDDDDHHHHHHH", "HHHHHNCCCCCCNJJJJJJJJJJJJJJDDCCCDHHHHHHH",
    "HHHHHNJJJJJJNJJJJJJJJJJJJJJDRRRRRRHHHHHH", "HHHHHNBBBBBBNBBBBBBBBBBBBBBDDRRRRRRHHHHH",
    "HHHHHBBBBBBBBBBBBBBBBBBBBBBDBDRRRRRRHHHH", "HHHHBBBBBBBBBBBBBBBBBBBBBBBBBBDRRRRRRHHH",
    "HHHBBBBBBBBBBBBBBBBBBBBBBBBBBBBDRRRRRRHH", "HHBBBBBBBBBBBBBBBBBBBBBBBBBBBBBDDDDDDDDH",
    "HBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBDBBBBBBDH"
};

// 스프라이트 그리기 함수
void draw_sprite(int worldX, int worldY, const char* sprite[], int h, int cameraLeftX, int cameraTopY) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; sprite[y][x] != '\0'; x++) {
            int sx = worldX - cameraLeftX + x;
            int sy = worldY - cameraTopY + y; // sx, sy 가 중요함. 카메라가 움직여도 그림들이 제자리에 가만히 있도록 해줌.
            char c = sprite[y][x];

            switch (c) {
            case 'R': draw_pixel(sx, sy, 200, 40, 40);   break;
            case 'Y': draw_pixel(sx, sy, 230, 220, 120); break;
            case 'W': draw_pixel(sx, sy, 100, 180, 255); break;
            case 'D': draw_pixel(sx, sy, 120, 70, 20);   break;
            case 'G': draw_pixel(sx, sy, 80, 200, 80);   break;
            case 'C': draw_pixel(sx, sy, 255, 255, 255); break;
            case 'N': draw_pixel(sx, sy, 0, 0, 0);       break;
            case 'Q': draw_pixel(sx, sy, 190, 190, 190); break;
            case 'B': draw_pixel(sx, sy, 165, 135, 100); break;
            case 'H': draw_pixel(sx, sy, 200, 200, 200); break;
            case 'J': draw_pixel(sx, sy, 210, 210, 210); break;
            case 'M': draw_pixel(sx, sy, 25, 25, 112); break;
            case 'L': draw_pixel(sx, sy, 47, 79, 79); break;
            }
        }
    }
}




// 인게임 마을 맵 함수
int game_map(GameState* state) {
    Force_English_Mode();
    printf("\033[?25l\033[2J");
    int playerX = 13;
    int playerY = 26;
    int moveSpeed = 1;

    int cameraLeftX = 0;
    int cameraTopY = 0;

    draw_sprite(0, 0, SKY, 28, cameraLeftX, cameraTopY);
    draw_sprite(1, 19, HOUSE, 8, cameraLeftX, cameraTopY);
    draw_sprite(30, 19, PHARMACY, 8, cameraLeftX, cameraTopY);
    draw_pixel(13, 26, 255, 255, 0);

    while (1) {
        Sleep(22);
        int oldPlayerX = playerX;
        int oldPlayerY = playerY;

        if (GetAsyncKeyState('A') & 0x8000)  playerX -= moveSpeed;
        if (GetAsyncKeyState('D') & 0x8000)  playerX += moveSpeed;
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return 7;

        if (playerX < 0) playerX = 0;
        if (playerX > WORLD_W - 1) playerX = WORLD_W - 1;

        if (oldPlayerX != playerX || oldPlayerY != playerY) {
            char bg_char = 'W';

            if (oldPlayerY >= 0 && oldPlayerY < 28 && oldPlayerX >= 0 && oldPlayerX < 40) {
                bg_char = SKY[oldPlayerY][oldPlayerX];
            }
            if (oldPlayerX >= 1 && oldPlayerX < 1 + 11 && oldPlayerY >= 19 && oldPlayerY < 19 + 8) {
                char hc = HOUSE[oldPlayerY - 19][oldPlayerX - 1];
                if (hc != '.') bg_char = hc;
            }
            if (oldPlayerX >= 30 && oldPlayerX < 30 + 9 && oldPlayerY >= 19 && oldPlayerY < 19 + 8) {
                char pc = PHARMACY[oldPlayerY - 19][oldPlayerX - 30];
                if (pc != '.') bg_char = pc;
            }

            int old_sx = oldPlayerX - cameraLeftX;
            int old_sy = oldPlayerY - cameraTopY;

            if (bg_char == 'W') draw_pixel(old_sx, old_sy, 100, 180, 255);
            // 다른 배경 타일 복구 로직이 필요하다면 여기에 추가.

            int p_screen_x = playerX - cameraLeftX;
            int p_screen_y = playerY - cameraTopY;
            draw_pixel(p_screen_x, p_screen_y, 255, 255, 0);
        }

        Sleep(16);

        Move_Cursor(1, SCR_H + 2);
        printf("======================================================");
        Move_Cursor(1, SCR_H + 3);
        printf("POSITION : (%3d, %3d) | DAY : %d | MONEY : %d원    ", playerX, playerY, state->day, state->money);

        if (playerX <= 10) { // 집으로 입장 조건 완화
            printf("\033[?25h\033[2J\033[1;1H");
            return 5;
        }
        if (playerX >= 30 ) { // 약국으로 입장 조건 완화
            printf("\033[?25h\033[2J\033[1;1H");
            Move_Cursor(1, SCR_H + 5);
            printf("오늘의 약국영업을 시작합니다.");
            Sleep(1200);
            
            return 6;
        }
    }
}

// 인게임 마을 맵 함수
int game_map_night(GameState* state) {
    Force_English_Mode();
    printf("\033[?25l\033[2J");
    Force_English_Mode();
    int playerX = 27;
    int playerY = 26;
    int moveSpeed = 1;

    int cameraLeftX = 0;
    int cameraTopY = 0;

    draw_sprite(0, 0, SKY_NIGHT, 28, cameraLeftX, cameraTopY);
    draw_sprite(1, 19, HOUSE, 8, cameraLeftX, cameraTopY);
    draw_sprite(30, 19, PHARMACY, 8, cameraLeftX, cameraTopY);
    draw_pixel(26, 26, 255, 255, 0);

    while (1) {
        Sleep(22);
        int oldPlayerX = playerX;
        int oldPlayerY = playerY;

        if (GetAsyncKeyState('A') & 0x8000)  playerX -= moveSpeed;
        if (GetAsyncKeyState('D') & 0x8000)  playerX += moveSpeed;
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return 7;

        if (playerX < 0) playerX = 0;
        if (playerX > WORLD_W - 1) playerX = WORLD_W - 1;

        if (oldPlayerX != playerX || oldPlayerY != playerY) {
            char bg_char = 'W';

            if (oldPlayerY >= 0 && oldPlayerY < 28 && oldPlayerX >= 0 && oldPlayerX < 40) {
                bg_char = SKY_NIGHT[oldPlayerY][oldPlayerX];
            }
            if (oldPlayerX >= 1 && oldPlayerX < 1 + 11 && oldPlayerY >= 19 && oldPlayerY < 19 + 8) {
                char hc = HOUSE[oldPlayerY - 19][oldPlayerX - 1];
                if (hc != '.') bg_char = hc;
            }
            if (oldPlayerX >= 30 && oldPlayerX < 30 + 9 && oldPlayerY >= 19 && oldPlayerY < 19 + 8) {
                char pc = PHARMACY[oldPlayerY - 19][oldPlayerX - 30];
                if (pc != '.') bg_char = pc;
            }

            int old_sx = oldPlayerX - cameraLeftX;
            int old_sy = oldPlayerY - cameraTopY;

            if (bg_char == 'M') draw_pixel(old_sx, old_sy, 25, 25, 112);
            // 다른 배경 타일 복구 로직이 필요하다면 여기에 추가.

            int p_screen_x = playerX - cameraLeftX;
            int p_screen_y = playerY - cameraTopY;
            draw_pixel(p_screen_x, p_screen_y, 255, 255, 0);
        }

        Sleep(16);

        Move_Cursor(1, SCR_H + 2);
        printf("======================================================");
        Move_Cursor(1, SCR_H + 3);
        printf("POSITION : (%3d, %3d) | DAY : %d | MONEY : %d원    ", playerX, playerY, state->day, state->money);

        if (playerX <= 10) { // 집으로 입장 조건 완화
            printf("\033[?25h\033[2J\033[1;1H");
            return 5;
        }
        if (playerX >= 30) { // 약국으로 입장 조건 완화
            printf("\033[?25h\033[2J\033[1;1H");
            Move_Cursor(1, SCR_H + 5);
            printf("더 영업하기엔 시간이 늦었다 이만 돌아가자");
            Sleep(1000);
           
            return 9;
        }
    }
}



// 집 내부 함수
int house(GameState* state) {
    Force_English_Mode();
    printf("\033[?25l\033[2J");
    mciSendString(TEXT("setaudio Room-bgm volume to 80"), NULL, 0, NULL);
    mciSendString(TEXT("play Room-bgm repeat"), NULL, 128, NULL);

    int selectX = 1, selectY = 1;
    if (state->opend == 0) {
    draw_sprite(20, 5, ROOM, 25, 0, 0);
    }
    else {
        draw_sprite(20, 5, ROOM_NIGHT, 25, 0, 0);
    }
    Move_Cursor(40, 30);
    printf("--------------------------------------------------------------------------------");

    while (_kbhit()) {
        _getch();
    }

    while (1) {
        // UI 출력 최적화
        Move_Cursor(55, 34);
        if (selectX == 1 && selectY == 1) { set_color(BG_COLOR_YELLOW); printf("1.잠을 잔다 "); printf(COLOR_RESET); }
        else printf("1.잠을 잔다 ");

        Move_Cursor(90, 34);
        if (selectX == 2 && selectY == 1) { set_color(BG_COLOR_YELLOW); printf("2.피아노를 연주한다 "); printf(COLOR_RESET); }
        else printf("2.피아노를 연주한다 ");

        Move_Cursor(53, 36);
        if (selectX == 1 && selectY == 2) { set_color(BG_COLOR_YELLOW); printf("3.밖으로 나간다 "); printf(COLOR_RESET); }
        else printf("3.밖으로 나간다 ");

        Move_Cursor(90, 36);
        if (selectX == 2 && selectY == 2) { set_color(BG_COLOR_YELLOW); printf("4.당일 발송 약사 몰"); printf(COLOR_RESET); }
        else printf("4.당일 발송 약사 몰");

        int input = _getch(); // 키보드 입력 받기

        // 1. 방향키 (특수 키) 방어막
        if (input == 224 || input == 0) {
            _getch(); // 방향키 프리픽스(추가 정보) 버림
            continue;
        }

        // 2. 한글 'ㅈ, ㅁ, ㄴ, ㅇ' 입력 처리 (UTF-8 해킹!)
        // ㅈ(w), ㅁ(a), ㄴ(s), ㅇ(d)의 첫 바이트는 무조건 227!
        if (input == 227) {
            int b2 = _getch();
            int b3 = _getch();

            if (b2 == 133 && b3 == 136 && selectY != 1) selectY -= 1;      // ㅈ
            else if (b2 == 133 && b3 == 129 && selectX != 1) selectX -= 1; // ㅁ
            else if (b2 == 132 && b3 == 180 && selectY != 2) selectY += 1; // ㄴ
            else if (b2 == 133 && b3 == 135 && selectX != 2) selectX += 1; // ㅇ

            // 뒤에 딸려온 쓰레기 완벽 청소
            while (_kbhit()) _getch();
            continue;
        }

        // 3. 영문 'WASD' 정상 처리
        if ((input == 'w' || input == 'W') && selectY != 1) selectY -= 1;
        if ((input == 'a' || input == 'A') && selectX != 1) selectX -= 1;
        if ((input == 's' || input == 'S') && selectY != 2) selectY += 1;
        if ((input == 'd' || input == 'D') && selectX != 2) selectX += 1;
        // 엔터키(13) 입력 시 처리 (GetAsyncKeyState 혼용 버그 제거)
        if (input == 13) {
            if (selectX == 1 && selectY == 1) {
                state->day += 1;
                state->opend = 0;
                system("cls");
                Move_Cursor(40, 15);
                printf("지루한 하루가 지나고... [%d일차] 아침이 밝았습니다.", state->day);
                Sleep(2000);
                printf("\033[?25h\033[2J\033[1;1H");
                mciSendString(TEXT("stop Room-bgm"), NULL, 128, NULL);
                return 5;
            }
            if (selectX == 2 && selectY == 1) {
                mciSendString(TEXT("pause Room-bgm"), NULL, 128, NULL);
                mciSendString(TEXT("play piano"), NULL, 128, NULL);
                Sleep(4700);
                mciSendString(TEXT("seek piano to start"), NULL, 0, NULL);
                mciSendString(TEXT("resume Room-bgm"), NULL, 128, NULL);
            }
            if (selectX == 1 && selectY == 2) {
                mciSendString(TEXT("stop Room-bgm"), NULL, 128, NULL);
                mciSendString(TEXT("seek Room-bgm to start"), NULL, 0, NULL);
                return 4;
            }
            if (selectX == 2 && selectY == 2) {
                return 10;
            }

        }
    }
}

int drug_store(GameState* state) {
    int selectY = 0;       // 0 ~ 6 (선택한 약의 배열 인덱스)
    int selectX = 1;       // 1 ~ 99 (선택한 약의 구매 수량)
    int drug_select = 0;   // 0: 약 선택 모드 (W/S), 1: 수량 선택 모드 (A/D)

    Force_English_Mode();  // 입력 충돌 방지용

    while (1) {
        system("cls");

        Move_Cursor(2, 2);
        printf("=== [ 상점 구매 창 ] ===");
        Move_Cursor(2, 4);
        printf("보유 자금: %d 골드", state->money);

        // ★ 헬레나님의 우아한 배열 출력 로직 (하드코딩 금지!)
        for (int i = 0; i < 7; i++) {
            Move_Cursor(2, 6 + i);

            // 현재 내 커서(selectY)가 위치한 약이라면?
            if (i == selectY) {
                set_color(BG_COLOR_BLUE); // 하이라이트 효과
                printf(" ▶ %s : %d 골드 ", stock[i].name, stock[i].buy_price);

                // 게다가 수량 선택 모드(drug_select == 1)라면 개수 조절 UI 추가!
                if (drug_select == 1) {
                    printf("   [ 수량: ◀ %2d ▶ ] ", selectX);
                }
                printf(COLOR_RESET);
            }
            // 커서가 없는 나머지 약들은 평범하게 출력
            else {
                printf("    %s : %d 골드 ", stock[i].name, stock[i].buy_price);
            }
        }

        // 조작키 안내
        Move_Cursor(2, 15);
        if (drug_select == 0) {
            printf("[ W/S : 위아래 이동 ]  [ Enter : 수량 선택 ]  [ ESC : 상점 나가기 ]");
        }
        else {
            printf("[ A/D : 수량 조절 ]  [ Enter : 구매 확정 ]  [ ESC : 선택 취소 ]");
        }

        // 입력 찌꺼기 소각
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        int input = _getch();

        // 한글 ㅈㅁㄴㅇ 방향키 마법 (W, A, S, D로 변환)
        if (input == 227) {
            int b2 = _getch(); int b3 = _getch();
            if ((b2 == 133 && b3 == 136) || (b2 == 132 && b3 == 181)) input = 'w';
            else if (b2 == 133 && b3 == 129) input = 'a';
            else if (b2 == 132 && b3 == 180) input = 's';
            else if (b2 == 133 && b3 == 135) input = 'd';
            while (_kbhit()) _getch();
        }

        // 1. ESC 키 처리 (상황에 따라 다르게 작동)
        if (input == 27) {
            if (drug_select == 1) {
                drug_select = 0; // 수량 선택 중에 누르면 개수 선택만 취소함
            }
            else {
                return 5; // 약 선택 중에 누르면 상점 밖(집)으로 나감
            }
        }

        // 2. Enter 키 처리 (13)
        if (input == 13) {
            if (drug_select == 0) {
                // 아직 약만 골랐다면 수량 모드로 진입
                drug_select = 1;
                selectX = 1; // 개수는 무조건 1개부터 시작
            }
            else {
                // 수량까지 다 고르고 엔터를 쳤다면 구매 확정!
                int total_cost = stock[selectY].buy_price * selectX;

                if (state->money >= total_cost) {
                    state->money -= total_cost;          // 지갑에서 돈 빼고
                    state->inventory[selectY] += selectX; // 가방에 약 넣기!

                    Move_Cursor(2, 17);
                    printf("%s %d개를 %d골드에 구매했다!", stock[selectY].name, selectX, total_cost);
                    Sleep(1000); // 1초간 보여줌
                    drug_select = 0; // 구매 끝났으니 다시 기본 상태로 복귀
                }
                else {
                    Move_Cursor(2, 17);
                    printf("돈이 부족하잖아! 거지 녀석아!");
                    Sleep(1000);
                }
            }
        }

        // 3. 이동 키 처리 (상태에 따라 제한 구역을 완벽하게 통제)
        if (drug_select == 0) {
            // 약 선택 모드일 때는 위아래(W/S)만 가능하게 통제 (0 ~ 6번 방어)
            if ((input == 'w' || input == 'W') && selectY > 0) selectY--;
            if ((input == 's' || input == 'S') && selectY < 6) selectY++;
        }
        else {
            // 수량 선택 모드일 때는 좌우(A/D)만 가능하게 통제 (1개 ~ 99개)
            if ((input == 'a' || input == 'A') && selectX > 1) selectX--;
            if ((input == 'd' || input == 'D') && selectX < 99) selectX++;
        }
    }
}


// 게임 인트로 시작 함수
int game_Start() {
    mciSendString(TEXT("close myBgm"), NULL, 0, NULL);
    system("cls");
    Sleep(1000);
    return 5;
}

// 약국 영업 로직 함수
int main_game(GameState* state) {
    Force_English_Mode();

    if (state->opend == 0) {
        char medicine[50];
        int customer_count = 0;

        Sleep(1500);
        // 손님 수를 2명에서 5명으로 확장, 손님이랑 대사는 원하는거로 바꾸렴.
        CustomerProfile characters[6] = {
            {
                "txts/elena.txt", "txts/elena_fail.txt","txts/elena_ok.txt",
                
                {
                    {"엘레나: 야. 약국 주인. 머리가 너무 아파.화학식이 C8H9NO2 에다가 분자량이 151에 근접한거 좀 줘",
                    "두통약",
                    "엘레나: 하 꽤나 똑똑하군",
                    "엘레나: 니 녀석 간단한 화학식도 모르는거냐?"
                },
                {   "엘레나: 콜록.. 아이 씨. 갑자기 찬바람이 불어선. 아세트아미노펜 있어?",
                    "해열제",
                    "엘레나: 드디어 살거같네",
                    "엘레나: 실패대사2 "
                },
                {"엘레나: 수인 놈들이 또 분탕을.. 뭐든 좋으니까 자양강장제 좀 부탁할게.",
                 "회복포션",
                 "엘레나: 오늘 수인녀석을 몰아내겠다",
                 "엘레나: (속닥속닥) 아멜리아, 저녀석 모나티엄으로 납치해."}
                }, 3
            },
            {
                "txts/comi.txt", "txts/comi_fail.txt", "txts/comi_ok.txt",
              
                {
                    {"코미: 하암~ 잠을 너무 잤더니 머리아파.",
                    "두통약",
                    "코미: 잘했어. 코미가 칭찬 스티커 하나 붙여줄게.",
                    "코미: 이런것도 처방이라 내린거야? 수준 하고는."
                },
                {    "코미: 버터가 또 이상한거 주워먹다가 앓아 누웠어. 치료제 내놔.",
                    "해독제",
                    "코미: 그래. 바로 이거야. ",
                    "코미: 버터보다 멍청한 애가 더 있을 줄이야.."
                },
                {"코미: 배가 너무 아파. 코미는 사료 캔 3캔 먹었을 뿐인데.. .",
                 "소화제",
                 "코미: 너. 뭐 좀 아는 녀석이구나.",
                 "코미: 거기 너. 비켜. 코미가 약사하는게 더 좋을거야."}
                }, 3
            },
            // [추가 손님 1] 네르 (그림 파일은 임시로 elena 재활용, 대사 3개 추가)
            {
                "txts/neru.txt", "txts/neru_fail.txt", "txts/neru_ok.txt",

                {
                    {"네르: 어제 해당국을 너무먹어서 그런지 속이 너무 더부룩 한거 같아요.",
                     "소화제",
                     "네르: 와아~ 덕분에 한결 나아졌네요.",
                     "네르: 제가 말한건 이게 아니긴 합니다만... 뭐. 별 수 없죠."
                },
                     
                    {"네르: 여왕님이 떼스시는거 달래느라너무 피곤하답니다..",
                    "회복포션",
                    "네르 : 역시 당신이군요. 교주님께도 보고드려야 겠어요.",
                     "네르 : 여왕님 하나만으로도 골치 아픈데 당신까지."},

                    {"네르: 오늘따라 기도드리기가 힘드네요..잠이 잘 오게 해주세요.",
                    "수면제",
                     "네르: 고마워요. 다음에 또 부탁드릴께요.",
                     "네르: 니녀석 이단이구나!!!"}
                }, 3
            },
            // [추가 손님 2] 아야 (그림 파일은 임시로 comi 재활용, 대사 3개 추가)
            {
                "txts/aya.txt", "txts/aya_fail.txt", "txts/aya_ok.txt",

                {
                    {"아야: 어제 너무 격렬하게 움직여서 몸이 달아올랐어..",
                    "해열제",
                    "아야: 아아 편해졌어",
                    "아야: 으윽 몸이 더 달아오르잖아"},


                    {"아야: 오늘 밤 자는 것좀 도와줄래?",
                    "수면제",
                    "아야: 고마워 오늘 밤 기대할께",
                    "아야: 으윽 오늘밤은 못자겠네"}
                    ,
                    {"아야: 아앗..어제 질척거리는 무언가를 먹었어 너의 그것으로 해결해줘",
                    "해독제",
                    "아야: 우웁 꿀꺽 으으.. 비리긴 하지만 효과는 좋네",
                    "아야: 앗 따가워 이렇게 따가운건 처음이야"}
                }, 3
            },
            // [추가 손님 3] 림 (그림 파일은 임시로 elena 재활용, 대사 3개 추가)
            {
                "txts/rim2.txt", "txts/rim_fail.txt", "txts/rim_ok.txt",

                {
                    {"림: 재치있는 나는..엣취!..",
                    "감기약",
                    "림: 웰던. 당신은 정말 놀라운 사람이었던.",
                    "림: 이이제이는 이의제기할겁니다...!"},
                    {"림: 잠이 오지 않을 때는 잠자리를 잡아. 후후..",
                    "수면제",
                    "림: 으흐흐흐~",
                    "림: 약국은 절대 악국."},


                    {"림: 뜨거운 아이스 아메리카노 하나 줘", 
                     "해열제",
                     "림: 크후후후...",
                      "림: 이것이야말로 나의 말로...?"}
                }, 3
            },

            {
              "txts/spiki.txt", "txts/spiki_fail.txt", "txts/spiki_ok.txt",

                {
                    {"스핔이: 스핔이!", "회복포션", "스핔이: 쪼아요!", "스핔이: 흐에에엥"},
                    {"스핔이: 네르지마세요!", "두통약", "스핔이: 쪼아요1", "스핔이: 네르는 이렇게 폭력적인 역할이 아니란말이에요!"},
                    {"스핔이: 오 신이시여 정말 감사합니다 이런곳에서 일반인을 만나봽다니 저로말할거같으면....", "감기약", "오 정말 감사합니다 덕분에 한결 편해졌네요", "이런 안타깝군요 당신은 명의가 아니군요"}
                },3
            }
        };

        srand((unsigned int)time(NULL));
        //char* goodLines[3] = { "오! 몸이 아주 가뿐해졌어!", "역시 명의로구나.", "상태가 좋아졌군!" };
        //char* badLines[3] = { "으윽, 몸이 더 이상해졌잖아!", "잘못 준 것 같은데...", "돌팔이 녀석!" };


        time_t start_time = time(NULL); // 영업 시작 시간 기록
        int time_limit = 120;           // 영업시간 제한: 180초 (3분)

        while (1) {
            printf("\033[?25h");

            int elapsed_time = (int)(time(NULL) - start_time);
            int remain_time = time_limit - elapsed_time;

            // 남은 시간이 0 이하면 영업 종료! (루프 탈출)
            if (remain_time <= 0) {
                break;
            }



            while (_kbhit()) { // 지도에서 입력하고 남은 버퍼 청소
                _getch();
            }


            int wait_time = (rand() % 10) + 1;





            // -------------------------------------------------------------------------
            // [대기 화면 UI 개편] 우측 여백을 화려한 네온 스타일 테두리와 장식으로 채움
            // -------------------------------------------------------------------------
            system("cls");
            set_color(FONT_COLOR_WHITE);


            for (int i = wait_time; i > 0; i--) {
                printf("\033[?25h");
                int elapsed = (int)(time(NULL) - start_time);
                int remain_time = time_limit - elapsed;

                // 대기 도중에 3분(180초)이 다 지나버렸다면 얄짤없이 즉시 루프 탈출!
                if (remain_time <= 0) break;

                system("cls");

                Move_Cursor(82, 3);  printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
                Move_Cursor(82, 4);  printf("┃      "); set_color(BG_COLOR_BLUE); printf(" [+] ELIAS PHARMACY [+] "); printf(COLOR_RESET); printf("");
                Move_Cursor(82, 5);  printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
                Move_Cursor(82, 6);  printf("┃                                                        ");
                Move_Cursor(82, 7);  printf("┃  [STATUS BOARD]                                     ");
                Move_Cursor(82, 8);  printf("┃  - 현재 영업일 : "); set_color(BG_COLOR_RED); printf(" %2d 일차 ", state->day); printf(COLOR_RESET); printf("");
                Move_Cursor(82, 9);  printf("┃  - 보유 자금   : %10d 원", state->money);
                Move_Cursor(82, 10); printf("┃  - 마을 만족도 : [");
                // 만족도 바(Bar) 차트 시각화 효과
                int barcount = state->satisfaction / 10;
                for (int b = 0; b < 10; b++) {
                    if (b < barcount) printf("■");
                    else printf(" ");
                }
                printf("] %3d / 100                   ", state->satisfaction);
                Move_Cursor(82, 13); printf("┃                                                        ");
                Move_Cursor(82, 14); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
                Move_Cursor(82, 11);
                printf("┃ "); set_color(BG_COLOR_BLUE); printf("[영업 대기 중...] "); printf(COLOR_RESET);
                Move_Cursor(82, 12);
                printf("┃   ▶ 영업 종료까지 남은 시간 : %02d:%02d", remain_time / 60, remain_time % 60);

                Sleep(1000); // 정확히 1초 대기하면서 루프 회전
            }

            // 손님 및 대사 무작위 선택
            int customerIndex = rand() % 6;
            CustomerProfile* customer = &characters[customerIndex];
            int demandIndex = rand() % customer->num_demands;
            Demand currentDemand = customer->demands[demandIndex];

            // 아스키 아트 그리기 (왼쪽 화면)
            system("cls");
            draw_ascii_art(customer->chara, 5, 0);
            printf(COLOR_RESET);
            Sleep(600);

            // -------------------------------------------------------------------------
            // [인게임 상호작용 UI 개편] 손님 입장 후 화려하게 바뀌는 라이브 화면
            // -------------------------------------------------------------------------
            Move_Cursor(82, 3);  printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
            Move_Cursor(82, 4);  printf("┃      "); set_color(BG_COLOR_RED); printf(" [*] 손님 받아라! [*] "); printf(COLOR_RESET); printf("       ");
            Move_Cursor(82, 5);  printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
            Move_Cursor(82, 6);  printf("┃                                                       ");
            Move_Cursor(82, 7);  printf("┃  [STATUS BOARD]                                   ");
            Move_Cursor(82, 8);  printf("┃  - 현재 영업일 : %2d 일차                             ", state->day);
            Move_Cursor(82, 9);  printf("┃  - 보유 자금   : %10d 원                              ", state->money);
            Move_Cursor(82, 10); printf("┃  - 마을 만족도 : %3d / 100                            ", state->satisfaction);
            Move_Cursor(82, 11); printf("┃                                                       ");
            Move_Cursor(82, 12); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");

            // 손님 정보 및 대사창 영역 (남아도는 공간 채우기)
// 손님 정보 및 대사창 영역
            Move_Cursor(82, 14); set_color(BG_COLOR_BrRED); printf(" ★ 손님이 원하는 약을 찾아라! ★ "); printf(COLOR_RESET);
            Move_Cursor(82, 16); printf(" > 손님의 하소연 : ");

            // ★ 헬레나님의 자동 개행 함수 사용! (40칸 제한)
            print_dialogue_wrapped(84, 17, currentDemand.dialogue, 92);

            // ==============================================================
            // ★ 하소연이 길어질 것을 대비해 UI 전체를 3칸 아래로 밀어냄! (Y좌표 +3)
            // ==============================================================
            Move_Cursor(82, 22); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
            Move_Cursor(82, 23); printf("┃  [ 현재 가방 속 재고 ]");
            Move_Cursor(82, 24);
            printf("┃  ");
            for (int i = 0; i < 7; i++) {
                if (state->inventory[i] == 0) set_color(FONT_COLOR_RED);
                else set_color(FONT_COLOR_BrGREEN);
                printf("%s:%d ", stock[i].name, state->inventory[i]);
            }
            printf(COLOR_RESET);

            // ==============================================================
            // 처방전 작성 입력칸 (마찬가지로 Y좌표 +3)
            // ==============================================================
            Move_Cursor(82, 26); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
            Move_Cursor(82, 27); printf("┃  [처방전 작성]                                   ");
            Move_Cursor(82, 28); printf("┃ ▶ 판매할 약 이름을 정확히 입력하세요:             ");
            Move_Cursor(82, 29); printf("┃                                                  ");
            Move_Cursor(82, 30); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");

            // 입력 커서를 새로운 처방전 입력칸(28번째 줄)으로 이동
            Move_Cursor(123, 28);

            printf("\033[?25h");
            Force_Korean_Mode();
            setbuf(stdin, NULL);
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
            scanf_s("%49s", medicine, (unsigned)_countof(medicine));
            while (getchar() != '\n');

            // ==============================================================
            // ★ 헬레나님의 절대 무적 판정 시스템 (재고 연동 완벽 반영)
            // ==============================================================

            // 1. 플레이어가 정답을 맞췄는지 확인
            int is_correct = (strcmp(medicine, currentDemand.answer) == 0);

            // 2. 입력한 약이 우리 카탈로그(stock)에 있는 약인지, 몇 번째 칸인지 검색
            int item_index = -1;
            for (int i = 0; i < 7; i++) {
                if (strcmp(medicine, stock[i].name) == 0) {
                    item_index = i;
                    break;
                }
            }

            if (is_correct) {
                // 정답은 맞췄는데, 가방에 약이 1개 이상 있는지 검사!
                if (item_index != -1 && state->inventory[item_index] > 0) {
                    // ★ 진정한 처방 성공! (정답 + 재고 있음)
                    draw_ascii_art(customer->success_chara, 5, 0);

                    int profit = stock[item_index].sell_price; // 구조체에 분리해둔 판매가 적용!
                    state->inventory[item_index] -= 1;         // 가방에서 약 1개 꺼내기
                    state->money += profit;                    // 지갑에 돈 넣기
                    state->satisfaction += 5;

                    Move_Cursor(82, 29); printf(" 【 판정 결과 】 ");
                    set_color(BG_COLOR_YELLOW); set_color(FONT_COLOR_BLACK);
                    printf(" ★ 처방 성공! (+%d원 / 만족도 +5) ★ ", profit); printf(COLOR_RESET);
                    Move_Cursor(82, 31);
                    printf(" > %s", currentDemand.precise_success);
                }
                else {
                    // ★ 재고 부족 실패! (정답은 알지만 약이 없음)
                    draw_ascii_art(customer->fail_art_file, 5, 0);

                    Move_Cursor(82, 29); printf(" 【 판정 결과 】 ");
                    set_color(BG_COLOR_BrRED);
                    printf(" ! 처방 실패... 재고가 부족합니다! (만족도 -5) ! "); printf(COLOR_RESET);

                    // ★ 맞춤형 실패 대사 출력!
                    Move_Cursor(82, 31);
                    printf(" > %s", currentDemand.precise_fail);

                    state->satisfaction -= 5;
                }
            }
            else {
                // ★ 완전 오답 실패! (엉뚱한 약을 처방함)
                draw_ascii_art(customer->fail_art_file, 5, 0);

                Move_Cursor(82, 29); printf(" 【 판정 결과 】 ");
                set_color(BG_COLOR_BrRED);
                printf(" ! 처방 실패... 잘못된 약입니다 (-3,000원 / 만족도 -5) ! "); printf(COLOR_RESET);

                // ★ 맞춤형 실패 대사 출력!
                Move_Cursor(82, 31);
                printf(" > %s", currentDemand.precise_fail);

                state->money -= 3000;
                state->satisfaction -= 5;
            }

            // 콘솔 입력 버퍼 찌꺼기 청소
            while (_kbhit()) _getch();

            // 판정 결과 읽을 시간 3초 부여
            Sleep(3000);






            fflush(stdin); // 입력 스트림 청소
            setbuf(stdin, NULL);

            customer_count++;
        }

        
        system("cls");
        Move_Cursor(40, 15);
        printf("오늘 영업을 마감합니다. 밤이 깊었으니 집으로 돌아갑시다.");
        Force_English_Mode();
        Sleep(2000);
        state->opend = 1;
        return 9;
    }
}








// 크레딧 화면 함수 (1회 출력 후 ESC 대기)
int credit_Scr() {
    Force_English_Mode();
    int garo = 80, sero = 30; // 기본 콘솔 크기 백업
    CONSOLE_SCREEN_BUFFER_INFO gasebi;


    // 화면을 그리기 전, 최초 1회 콘솔 창의 크기를 획득.

    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &gasebi)) {
        garo = gasebi.srWindow.Right - gasebi.srWindow.Left + 1;
        sero = gasebi.srWindow.Bottom - gasebi.srWindow.Top + 1;
    }

    if (garo < 80) garo = 80;
    if (sero < 30) sero = 30;

    // 중앙 정렬 좌표 계산 (가로 70칸, 콘텐츠 높이 16칸 기준)
    int boxX = (garo - 70) / 2;
    int boxY = (sero - 16) / 2;
    if (boxX < 1) boxX = 1;
    if (boxY < 1) boxY = 1;


    // 화면 전체를 단 '한 번'만 깨끗이 지우고 크레딧을 그린다.
    // (반복문 밖에 있으므로 화면 깜빡임이 없음.)

    system("cls");
    set_color(FONT_COLOR_WHITE);

    // 상단 테두리
    Move_Cursor(boxX, boxY);     printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
    Move_Cursor(boxX, boxY + 1); printf("┃                           ■ CREDITS ■                              ┃");
    Move_Cursor(boxX, boxY + 2); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");

    // 본문 내용 구획 분할
    Move_Cursor(boxX, boxY + 4);  printf("  [TEAM LEADER / MAIN DEVELOPER]                                   ");
    Move_Cursor(boxX, boxY + 5);  //set_color(BG_COLOR_BrRED);
    printf("  ▶ 202619005 이 현 진                                                ");
    printf(COLOR_RESET);

    Move_Cursor(boxX, boxY + 7);  printf("  [GRAPHIC ART & CO-DEVELOPER]                                     ");
    //set_color(BG_COLOR_BrGREEN);
    Move_Cursor(boxX, boxY + 8);  printf("  - 202617641 강 보 민  │  - 202617100 허 규 빈                       ");
    //printf(COLOR_RESET);

    Move_Cursor(boxX, boxY + 10); printf("  [SPECIAL THANKS / COLLABORATORS]                                 ");
    //set_color(BG_COLOR_YELLOW); set_color(FONT_COLOR_BLACK);
    Move_Cursor(boxX, boxY + 11); printf("  - 이은석 교수님  │  - 에피드 게임즈 게임 \"트릭컬\"            ");
    //printf(COLOR_RESET);

    // 하단 테두리 및 안내 바
    set_color(FONT_COLOR_WHITE);
    Move_Cursor(boxX, boxY + 13); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");

    Move_Cursor(boxX, boxY + 15); set_color(BG_COLOR_BLUE);
    printf(" ▶ 타이틀 화면으로 돌아가려면 [ ESC 키 ]를 누르십시오. ");
    printf(COLOR_RESET);



    while (1) { //getch로 최적화 및 대기
        int key = _getch();

        if (key == 27) { // ESC 키가 들어오면 비로소 루프를 깨부수고 나간다.
            break;
        }
    }

    system("cls");
    return 0;
}







// 초기 로딩 및 세팅 함수
int loading() {
    system("cls");
    Move_Cursor(10, 5); printf("본 게임은 전체화면으로 플레이하는 것을 전제로 제작되었습니다 F11로 전체화면으로 전환해주시길 바랍니다.");
    Move_Cursor(10, 6); printf("또한 선택창 이동은 영문 약 이름 적기는 한글을 사용합니다.");
    Move_Cursor(10, 7); printf("이를 이해 하였으면 엔터 키를 눌러주십시오.");
    setbuf(stdin, NULL);
    (void)getchar();

    system("cls");
    FILE* logo = fopen("murimuri.txt", "r");
    char line[1024];

    mciSendString(TEXT("open \"mp3s/nerujimaseyo.mp3\" type mpegvideo alias logo"), NULL, 0, NULL);
    mciSendString(TEXT("open \"mp3s/voice 7 voice.mp3\" type mpegvideo alias myBgm"), NULL, 0, NULL);
    mciSendString(TEXT("open \"mp3s/Room-bgm.mp3\" type mpegvideo alias Room-bgm"), NULL, 0, NULL);
    mciSendString(TEXT("open \"mp3s/Piano Sound Effect.mp3\" type mpegvideo alias piano"), NULL, 0, NULL);

    if (logo) {
        while (fgets(line, sizeof(line), logo) != NULL) {
            printf("%s", line);
        }
        fclose(logo);
    }

    mciSendString(TEXT("play logo repeat"), NULL, 0, NULL);
    mciSendString(TEXT("play logo wait"), NULL, 0, NULL);
    mciSendString(TEXT("setaudio myBgm volume to 100"), NULL, 0, NULL);
    mciSendString(TEXT("play myBgm repeat"), NULL, 0, NULL);
    mciSendString(TEXT("close logo"), NULL, 0, NULL);

    return 0;
}







// 타이틀 화면 렌더링 함수 (가로/세로 동적 중앙 정렬)
int render_Title() {
    printf("\033[?25l\033[2J");
    Force_English_Mode();
    int select = 1;
    int garo = 80, sero = 30; // 기본 콘솔 크기 예외 대비 백업
    CONSOLE_SCREEN_BUFFER_INFO gasebi;

    Sleep(100);

    while (1) {
        system("cls");
        set_color(FONT_COLOR_WHITE);

        // 실시간으로 현재 콘솔 창의 가로(garo)와 세로(sero) 픽셀/문자 수를 획득.
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &gasebi)) {
            garo = gasebi.srWindow.Right - gasebi.srWindow.Left + 1;
            sero = gasebi.srWindow.Bottom - gasebi.srWindow.Top + 1;
        }

        // 안전선 확보: 창이 지나치게 작아지면 최소 크기로 고정.
        if (garo < 80) garo = 80;
        if (sero < 30) sero = 30;

        // 상단 장식 라인 중앙 정렬 (가로 너비 76칸 기준)
        int topBarX = (garo - 76) / 2;
        if (topBarX < 1) topBarX = 1;

        Move_Cursor(topBarX + 5, 2);  printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
        Move_Cursor(topBarX + 5, 3);  printf("┃  ■ VERSION: 1.75  │  LOCALIZATION: UTF-8  │  DESIGN BY 무리무리    ┃");
        Move_Cursor(topBarX + 5, 4);  printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");


        // 대형 영문 타이틀 기호 배치 및 상하좌우 완전 중앙 정렬
        // 가로 너비 68칸, 타이틀부터 메뉴까지의 전체 콘텐츠 높이는 약 15칸.
        // 세로 진입점(titleY)을 (sero - 15) / 2 로 설정하여 정중앙으로 내림.

        int titleX = (garo - 68) / 2;
        if (titleX < 1) titleX = 1;

        int titleY = (sero - 15) / 2;
        if (titleY < 6) titleY = 6; // 상단 바와 겹침 방지 가이드라인
        titleX = titleX + 6;
        set_color(BG_COLOR_BrGREEN);
        Move_Cursor(titleX, titleY);     printf("######  #        #  #####  #####    #    #  #####  #    #");
        Move_Cursor(titleX, titleY + 1); printf("#       #        #  #   #  #         #  #   #   #  #    #");
        Move_Cursor(titleX, titleY + 2); printf("####    #        #  #####  #####      ##    #   #  #    #");
        Move_Cursor(titleX, titleY + 3); printf("#       #        #  #   #      #      ##    #   #  #    #");
        Move_Cursor(titleX, titleY + 4); printf("######  ######   #  #   #  #####      ##    #####  ######");

        // 한글 서브 타이틀 정렬 (길이 약 48칸)
        int subTitleX = (garo - 48) / 2;
        Move_Cursor(subTitleX, titleY + 6);
        set_color(BG_COLOR_BLUE);
        printf("   [ ■ 엘 리 아 스 의  약 국   영 업  일 지 ■ ] ");
        set_color(BG_COLOR_BLACK);

        // 중앙 조작 안내 패널 정렬 (가로 너비 62칸, 타이틀에 연동되어 내려감)
        int guideX = (garo - 62) / 2;
        int guideY = titleY + 8;
        set_color(FONT_COLOR_WHITE);
        Move_Cursor(guideX + 1, guideY);     printf("┌────────────────────────────────────────────────────────────┐");
        Move_Cursor(guideX + 1, guideY + 1); printf("│   [CONTROLS] W/S : 선택 이동  │  ENTER : 결정              │");
        Move_Cursor(guideX + 1, guideY + 2); printf("└────────────────────────────────────────────────────────────┘");

        // 선택 메뉴 리스트 정렬 (가로 너비 16칸 기준, 함께 아래로 안착)
        int menuX = (garo - 16) / 2;
        int menuY = guideY + 4;

        Move_Cursor(menuX, menuY);
        if (select == 1) {
            set_color(BG_COLOR_YELLOW); printf(" ▶  GAME START "); printf(COLOR_RESET);
        }
        else {
            printf("     GAME START ");
        }

        Move_Cursor(menuX, menuY + 2);
        if (select == 2) {
            set_color(BG_COLOR_YELLOW); printf(" ▶  CREDIT     "); printf(COLOR_RESET);
        }
        else {
            printf("     CREDIT     ");
        }

        Move_Cursor(menuX, menuY + 4);
        if (select == 3) {
            set_color(BG_COLOR_YELLOW); printf(" ▶  EXIT       "); printf(COLOR_RESET);
        }
        else {
            printf("     EXIT       ");
        }

        // 하단 저작권 띠지 정렬 (전체 높이 바닥에 고정되도록 유지)
        int copyX = (garo - 65) / 2;
        int copyY = sero - 3;
        if (copyY < menuY + 6) copyY = menuY + 6;

        set_color(FONT_COLOR_WHITE);
        Move_Cursor(topBarX + 5, copyY);     printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
        Move_Cursor(copyX + 3, copyY + 1);   printf("Copyright 2026. TEAM KANGWON AI CONTENT ENG. All rights reserved.");

        // 키 입력 처리 파트 
        int input = _getch();

        // 1. 방향키(특수키) 방어막
        if (input == 224 || input == 0) {
            _getch();
            continue;
        }

        // 2. 헬레나님의 마술: 한글 ㅈ, ㄴ을 영어 w, s로 변조해 버리기!
        if (input == 227) {
            int b2 = _getch();
            int b3 = _getch();

            // ★ ㅈ(133, 136)을 누르거나, 겹받침 ㄵ(132, 181)이 들어와도 무조건 'w'로 속임!
            if ((b2 == 133 && b3 == 136) || (b2 == 132 && b3 == 181)) {
                input = 'w';
            }
            // ㄴ을 누르면 "얘 s 눌렀음" 하고 속임
            else if (b2 == 132 && b3 == 180) {
                input = 's';
            }

            // 뒤에 남은 한글 찌꺼기는 싹 태워버림
            while (_kbhit()) _getch();

            // 변조 실패 시 처음으로 돌려보냄
            if (input == 227) continue;
        }

        // 4. 변조가 끝났으니 기존 switch문은 아무것도 모른 채로 완벽하게 작동함!
        switch (input) {
        case 'w': case 'W':
            if (select > 1) select--;
            break;
        case 's': case 'S':
            if (select < 3) select++;
            break;
        case 13: // Enter
            if (select == 1) return 1;
            if (select == 2) return 3;
            if (select == 3) return 7;
            break;
        case 27: // ESC
            return 7;
        }
    }
}


int endings(GameState* state) {
    
    Force_English_Mode();
    int money = state->money;

    //월세 실패
    if (money < 300000) {
        system("cls");
        printf("돈이 부족하구나 죽어버리렴");
    }


    if (money > 300000 || money < 330000) {
        system("cls");
        printf("아슬아슬하게 모았구나");
    }


    if (money > 330001) {
        system("cls");
        printf("훌륭해");
    }





}





// 메인 루프 진입점
int main() {
    int selec_Op = -1;
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");
    HideCursor();
    Force_English_Mode();



    GameState player = { 1, 10000, 50, 0, {3, 3, 3, 3, 3, 3, 3} };


    while (1) {
        
        if (player.day >= 11 && selec_Op != 8) {
            selec_Op = 8;
        }

        switch (selec_Op) {
        case -1: selec_Op = loading();     break;
        case 0:  selec_Op = render_Title(); break;
        case 1:  selec_Op = game_Start();    break;
        case 3:  selec_Op = credit_Scr();   selec_Op = 0; break; // 크레딧 보고 타이틀로 복귀
        case 4:  selec_Op = game_map(&player); break;
        case 5:  selec_Op = house(&player);    break;
        case 6:  selec_Op = main_game(&player); break;
        case 7:  // [종료 확인 단계] 게임 종료 요청(7)이 들어왔을 때의 처리
            system("cls");
            printf("게임을 종료하시겠습니까?\n");
            printf("취소하려면 [ ESC 키 ]를, 종료하려면 [ 엔터 키 ]를 입력하십시오\n");

            while (1) {
                int exit_key = _getch();

                if (exit_key == 13) { // 엔터 키: 완전 종료
                    Move_Cursor(0, 21);
                    printf("프로그램을 종료합니다.\n");
                    return 0; // 프로그램 완전 종료
                }

                if (exit_key == 27) { // ESC 키: 종료 취소 후 이전(타이틀) 화면으로 복귀
                    selec_Op = 0; // 타이틀 화면 코드로 변경
                    break;        // 내부 키 입력 대기 루프를 탈출
                }
            }
            // 내부 루프를 탈출하면 switch문을 빠져나가, 외곽의 while(1) 루프에 의해 
            // `selec_Op = 0`(타이틀 화면)인 상태로 자연스럽게 되돌아감.
            break;
         case 8: selec_Op = endings(&player); break; 
         case 9: selec_Op = game_map_night(&player); break;
         case 10: selec_Op = drug_store(&player); break;
        }
    }

    return 0;
}
