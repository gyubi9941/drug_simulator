#define _CRT_SECURE_NO_WARNINGS           // 구동시 예외처리

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
#define FONT_COLOR_WHITE 37
#define FONT_COLOR_BLACK 30
#define COLOR_RESET      "\x1b[0m"

// 공통 전역 변수 구조체 선언
typedef struct {
    int day;
    int money;
    int satisfaction;
} GameState;

typedef struct {
    char dialogue[200];
    char answer[50];
} Demand;

typedef struct {
    char chara[256];
    char fail_art_file[256];
    char success_dialogue[3][200];
    char fail_dialogue[3][200];
    Demand demands[3];
    int num_demands;
} CustomerProfile;

// 커서 위치 이동 함수
void Move_Cursor(int x, int y) {
    printf("\033[%d;%dH", y, x);
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
void set_color(int code) {
    printf("\x1b[%dm", code);
}

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
            }
        }
    }
}

// 인게임 마을 맵 함수
int game_map(GameState* state) {
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
        if (playerX >= 30) { // 약국으로 입장 조건 완화
            printf("\033[?25h\033[2J\033[1;1H");
            return 6;
        }
    }
}

// 집 내부 함수
int house(GameState* state) {
    printf("\033[?25l\033[2J");
    mciSendString(TEXT("setaudio Room-bgm volume to 80"), NULL, 0, NULL);
    mciSendString(TEXT("play Room-bgm repeat"), NULL, 128, NULL);

    int selectX = 1, selectY = 1;
    draw_sprite(20, 5, ROOM, 25, 0, 0);
    Move_Cursor(40, 30);
    printf("--------------------------------------------------------------------------------");

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

        int input = _getch();

        if (input > 127 || input < 0) {
            while (_kbhit()) _getch();
            continue;
        }
        if (input == 224 || input == 0) {
            _getch(); // 방향키 프리픽스(입력된 값을 알려주는 추가 정보) 버림
            continue;
        }

        if ((input == 'w' || input == 'W') && selectY != 1) selectY -= 1;
        if ((input == 'a' || input == 'A') && selectX != 1) selectX -= 1;
        if ((input == 's' || input == 'S') && selectY != 2) selectY += 1;
        if ((input == 'd' || input == 'D') && selectX != 2) selectX += 1;

        // 엔터키(13) 입력 시 처리 (GetAsyncKeyState 혼용 버그 제거)
        if (input == 13) {
            if (selectX == 1 && selectY == 1) {
                state->day += 1;
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
    char medicine[50];
    int customer_count = 0;

    // 손님 수를 2명에서 5명으로 확장, 손님이랑 대사는 원하는거로 바꾸렴.
    CustomerProfile characters[5] = {
        {
            "txts/elena.txt", "txts/elena.txt",
            {"엘레나: 역시 맞출줄 알았어", "엘레나: 역시나 명의야", "엘레나: 대단하군"},
            {"엘레나: 히익 니녀석 기초적인 화학식도 모르는거냐?", "엘레나: 으앙! 이게 아니잖아! 배가 더 아파졌어!", "엘레나: 하 니녀석 출입금지시켜버린다"},
            {
                {"엘레나: 머리가 아파! 달콤하고 하얀 알약 줘!", "타이레놀"},
                {"엘레나: 배가 고파서 돌을 씹어먹었더니 배탈이 났어... 소화제 줘!", "소화제"},
                {"엘레나: 넘어져서 무릎이 까졌어! 빨간약 발라줘!", "빨간약"}
            }, 3
        },
        {
            "txts/comi.txt", "txts/comi.txt",
            {"코미: 후아암 역시 최고야", "코미: 오늘은 편하게 잘 수 있겠구나", "코미: 고마워"},
            {"코미: 이게뭐야! 이런 약사는 빨리 망해야해!", "코미: 느아앙 너 뭘 주는거야!", "코미: 오늘 낮잠은 글렀네"},
            {
                {"코미: 잠이 안 온다구... 포션 같은 거 없어?", "포션"},
                {"코미: 눈이 침침해... 인공눈물 줘...", "인공눈물"},
                {"코미: 쿨럭쿨럭... 감기 걸린 것 같아. 감기약 줘.", "감기약"}
            }, 3
        },
        // [추가 손님 1] 네르 (그림 파일은 임시로 elena 재활용, 대사 3개 추가)
        {
            "txts/elena.txt", "txts/elena.txt",
            {"네르: 교주님께도 보고드려야겠군요. 훌륭합니다.", "네르: 음, 약효가 아주 빠르군요.", "네르: 고맙습니다. 한 시름 놓았네요."},
            {"네르: 도대체 뭘 준 겁니까?! 당장 처벌하겠습니다!", "네르: 으윽... 사기꾼이 따로 없군요.", "네르: 이딴 걸 약이라고 내놓다니..."},
            {
                {"네르: 스트레스 때문에 뒷목이 당기는군요..", "혈압약"},
                {"네르: 요즘 서류를 너무 많이 봐서 피로해요..", "피로해소제"},
                {"네르: 여왕님께 설교하느라 입안이 다 헐어버렸답니다. 바르는 연고 좀 주세요.", "연고"}
            }, 3
        },
        // [추가 손님 2] 에르핀 (그림 파일은 임시로 comi 재활용, 대사 3개 추가)
        {
            "txts/comi.txt", "txts/comi.txt",
            {"에르핀: 우와아! 단숨에 다 나았어! 빵 먹으러 가야지!", "에르핀: 헤헤, 고마워! 넌 좋은 녀석이구나!", "에르핀: 역시 내 직감은 틀리지 않았어!"},
            {"에르핀: 아야야! 배가 더 아파! 너 나한테 독약 먹였지?!", "에르핀: 맛없어! 게다가 낫지도 않잖아!", "에르핀: 흐아앙! 이런거 말고 효과 직빵인거 내놓으란 말이야!!"},
            {
                {"에르핀: 빵을 너무 많이 먹었나 봐... 속이 더부룩해. 약 줘!", "까스활명수"},
                {"에르핀: 이가 너무 시리고 아파... 약 좀 줘봐.", "치통약"},
                {"에르핀: 숲에서 놀다가 벌에 쏘였어! 약 내놔!", "해독제"}
            }, 3
        },
        // [추가 손님 3] 벨벳 (그림 파일은 임시로 elena 재활용, 대사 3개 추가)
        {
            "txts/elena.txt", "txts/elena.txt",
            {"벨벳: 흠, 제법 쓸만한 약이군요.", "벨벳: 통증이 가시는군. 칭찬해주지.", "벨벳: 고맙군. 약값은 넉넉히 주지."},
            {"벨벳: 나를 모욕할 셈인가? 이딴 쓰레기를 주다니!", "벨벳: 크윽... 독을 탄 건 아니겠지?!", "벨벳: 당장 눈앞에서 치워라!"},
            {
                {"벨벳: 전투 중에 뼈가 삔 것 같다. 파스 하나 내오거라.", "파스"},
                {"벨벳: 상처가 덧나서 진물이 흐르는군. 소독약이 필요하다.", "소독약"},
                {"벨벳: 기침이 멈추질 않는군. 진해거담제 처방해라.", "진해거담제"}
            }, 3
        }
    };

    srand((unsigned int)time(NULL));
    char* goodLines[3] = { "오! 몸이 아주 가뿐해졌어!", "역시 명의로구나.", "상태가 좋아졌군!" };
    char* badLines[3] = { "으윽, 몸이 더 이상해졌잖아!", "잘못 준 것 같은데...", "돌팔이 녀석!" };

    while (customer_count < 3) {
        while (_kbhit()) { // 지도에서 입력하고 남은 버퍼 청소
            _getch();
        }
        // -------------------------------------------------------------------------
        // [대기 화면 UI 개편] 우측 여백을 화려한 네온 스타일 테두리와 장식으로 채움
        // -------------------------------------------------------------------------
        system("cls");
        set_color(FONT_COLOR_WHITE);

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
        Move_Cursor(82, 11); printf("┃                                                        ");
        Move_Cursor(82, 12); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
        Move_Cursor(82, 14); printf("   [! SYSTEM MESSAGE !] ");
        Move_Cursor(82, 15); set_color(BG_COLOR_YELLOW); set_color(FONT_COLOR_BLACK); printf("  [●] [ 대 기 중 ] 새로운 손님이 문 앞에 다다랐습니다.  "); printf(COLOR_RESET);
        Move_Cursor(82, 17); printf("   ▶ 진행하려면 [ 엔터 키 ]를 무림의 기세로 누르십시오.");

        setbuf(stdin, NULL);
        (void)getchar();

        // 손님 및 대사 무작위 선택
        int customerIndex = rand() % 5;
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
        Move_Cursor(82, 3);  printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
        Move_Cursor(82, 4);  printf("┃      "); set_color(BG_COLOR_RED); printf(" [*] 손님 받아라! [*] "); printf(COLOR_RESET); printf("       ");
        Move_Cursor(82, 5);  printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
        Move_Cursor(82, 6);  printf("┃                                                       ");
        Move_Cursor(82, 7);  printf("┃  [STATUS BOARD]                                   ");
        Move_Cursor(82, 8);  printf("┃  - 현재 영업일 : %2d 일차                             ", state->day); 
        Move_Cursor(82, 9);  printf("┃  - 보유 자금   : %10d 원                              ", state->money);
        Move_Cursor(82, 10); printf("┃  - 마을 만족도 : %3d / 100                            ", state->satisfaction);
        Move_Cursor(82, 11); printf("┃                                                       ");
        Move_Cursor(82, 12); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");

        // 손님 정보 및 대사창 영역 (남아도는 공간 채우기)
        Move_Cursor(82, 14); set_color(BG_COLOR_BrRED); printf(" ★ 손님이 원하는 약을 찾아라! ★ "); printf(COLOR_RESET);
        Move_Cursor(82, 16); printf(" > 손님의 하소연 : ");
        Move_Cursor(84, 18); set_color(FONT_COLOR_BLACK); printf("\033[107m %s \033[0m", currentDemand.dialogue); // 말풍선 느낌으로 반전 강조

        Move_Cursor(82, 21); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
        Move_Cursor(82, 22); printf("┃  [처방전 작성]                                   ");
        Move_Cursor(82, 23); printf("┃ ▶ 판매할 약 이름을 정확히 입력하세요:             ");
        Move_Cursor(82, 24); printf("┃                                                    ");
        Move_Cursor(82, 25); printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");

        // 입력 커서를 처방전 내부로 자연스럽게 이동시킴
        Move_Cursor(123, 23);

        setbuf(stdin, NULL);
        scanf_s("%49s", medicine, (unsigned)_countof(medicine));

        // 결과 처리 구역
        if (strcmp(medicine, currentDemand.answer) == 0) {
            Move_Cursor(82, 27); printf(" 【 판정 결과 】 ");
            set_color(BG_COLOR_YELLOW); set_color(FONT_COLOR_BLACK); printf(" ★ 처방 성공! (+5,000원 / 만족도 +5) ★ "); printf(COLOR_RESET);
            Move_Cursor(82, 29); printf(" > %s", customer->success_dialogue[rand() % 3]);
            state->money += 5000;
            state->satisfaction += 5;
        }
        else {
            Move_Cursor(82, 27); printf(" 【 판정 결과 】 ");
            set_color(BG_COLOR_BrRED); printf(" ! 처방 실패... (-3,000원 / 만족도 -5) ! "); printf(COLOR_RESET);
            Move_Cursor(82, 29); printf(" > %s", customer->fail_dialogue[rand() % 3]);
            state->money -= 3000;
            state->satisfaction -= 5;
        }

        // 하단 여백 채우기용 안내문
        Move_Cursor(82, 32); printf("────────────────────────────────────────────────────────");
        Move_Cursor(82, 33); printf(" ※ 다음 손님을 맞이하려면 [ 엔터 키 ]를 누르십시오... ");
        fflush(stdin); // 입력 스트림 청소
        setbuf(stdin, NULL);
        (void)getchar();

        customer_count++;
    }

    state->day += 1;
    system("cls");
    Move_Cursor(40, 15);
    printf("오늘 영업을 마감합니다. 밤이 깊었으니 집으로 돌아갑시다.");
    Sleep(2000);
    return 5;
}

// 크레딧 화면 함수 (1회 출력 후 ESC 대기)
int credit_Scr() {
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
        
        Move_Cursor(topBarX+5, 2);  printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
        Move_Cursor(topBarX+5, 3);  printf("┃  ■ VERSION: 1.75  │  LOCALIZATION: UTF-8  │  DESIGN BY 무리무리    ┃");
        Move_Cursor(topBarX+5, 4);  printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");

        
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
        Move_Cursor(guideX+1, guideY);     printf("┌────────────────────────────────────────────────────────────┐");
        Move_Cursor(guideX+1, guideY + 1); printf("│   [CONTROLS] W/S : 선택 이동  │  ENTER : 결정              │");
        Move_Cursor(guideX+1, guideY + 2); printf("└────────────────────────────────────────────────────────────┘");

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
        Move_Cursor(topBarX+5, copyY);     printf("◆━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━◆");
        Move_Cursor(copyX+3, copyY + 1);   printf("Copyright 2026. TEAM KANGWON AI CONTENT ENG. All rights reserved.");

        // 키 입력 처리 파트 
        int input = _getch();

        if (input > 127 || input < 0) {
            while (_kbhit()) _getch();
            continue;
        }

        switch (input) {
        case 'w': case 'W':
            if (select > 1) select--;
            break;
        case 's': case 'S':
            if (select < 3) select++;
            break;
        case 13:
            if (select == 1) return 1;
            if (select == 2) return 3;
            if (select == 3) return 7;
            break;
        case 27:
            return 7;
        }
    }
}

// 메인 루프 진입점
int main() {
    int selec_Op = -1;
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, ".UTF8");

    GameState player = { 1, 10000, 50 };

    while (1) {
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
        }
    }

    return 0;
}
