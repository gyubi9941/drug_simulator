#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> // stdio.h 파일 가져오기
#include <windows.h> // windows.h 파일 가져오기
#pragma execution_character_set("utf-8")
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib") // MSVC 컴파일러용


// 월드 좌표 지정
#define SCR_W 80  // X좌표 2칸이 1칸이므로 80 / 2 = 40
#define SCR_H 28  // 하단 UI 2줄을 제외한 30 - 2 = 28

// 월드 크기 정의
#define WORLD_W 100
#define WORLD_H 100
#define CONSOLE_H 30

//색지정
#define FONT_COLOR_BLACK  30
#define BG_COLOR_BLACK 40
#define FONT_COLOR_RED 31
#define BG_COLOR_RED 41
#define BG_COLOR_BLUE 44
#define BG_COLOR_YELLOW 43
#define FONT_COLOR_MAGENTA 45
#define BG_COLOR_BrRED 101
#define FONT_COLOR_WHITE 37

// 픽셀 그리기 함수 (X축 2칸을 1칸으로 처리하여 정사각형 만들기)
void draw_pixel(int x, int y, int r, int g, int b) {
    if (x < 0 || x >= SCR_W || y < 0 || y >= SCR_H) return;
    // 커서 이동 (1부터 시작하므로 +1) 후 RGB 배경색 지정 출력 후 리셋
    printf("\033[%d;%dH\033[48;2;%d;%d;%dm  \033[0m", y + 1, (x * 2) + 1, r, g, b);
}



void Move_Cursor(int x, int y) {

    printf("\033[%d;%dH", y, x);
}


#define COLOR_RESET "\x1b[0m"

/**
 * 폰트(글자) 색상을 변경하는 함수
 * @param code: 30-37 (기본), 90-97 (밝은 색)
 */
void set_color(int code) {
    printf("\x1b[%dm", code);
}

const char* PHARMACY[] = {
    "GGGGGGGGG",
    "GGGGRGGGG",
    "GGGGRGGGG",
    "GGRRRRRGG",
    "GGGGRGGGG",
    "GGGGRGGGG",
    "GGGGGGGGG",
    "GGGGGGGGG"
};

const char* HOUSE[] = {
    ".....R.....",
    "....RRR....",
    "...RRRRR...",
    "..RRRRRRR..",
    ".RRRRRRRRR.",
    ".YYYYYYYYY.",
    ".YYWYYYWYY.",
    ".YYYYDYYYY.",
    ".YYYYYYYYY."
};
const char* SKY[] = {
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWCCCWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWCCCCCWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWCCCCWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWCCCCCCCCWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWCCCWWWWWWWWWW",
    "WWWWWWWWWCCWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWCCCCCCCWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWCCCWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
    "DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD",
};
const char* ROOM[] = {
    "HHHQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQHHH",
    "HHHHQQQQQQQQQQQCCCCCCCCCCQQQQQQQQQQQHHHH",
    "HHHHHQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQHHHHH",
    "HHHHHHQQQQQQQQQQQQQQQQQQQQQQQQQQQQHHHHHH",
    "HHHHHHHQQQQQQQQQQQQQQQQQQQQQQQQQQHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJWWWWWWWWJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJWWWWWWWWJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJWWWWWWWWJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJDDDDDDDDDDJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHJJJJJJJJJJJJJJJJJJJJJJJJJJHHHHHHH",
    "HHHHHHHNNNNNNJJJJJJJJJJJJJJDDDDDDHHHHHHH",
    "HHHHHHNNNNNNNJJJJJJJJJJJJJJDDDDDDHHHHHHH",
    "HHHHHNCCCCCCNJJJJJJJJJJJJJJDDCCCDHHHHHHH",
    "HHHHHNJJJJJJNJJJJJJJJJJJJJJDRRRRRRHHHHHH",
    "HHHHHNBBBBBBNBBBBBBBBBBBBBBDDRRRRRRHHHHH",
    "HHHHHBBBBBBBBBBBBBBBBBBBBBBDBDRRRRRRHHHH",
    "HHHHBBBBBBBBBBBBBBBBBBBBBBBBBBDRRRRRRHHH",
    "HHHBBBBBBBBBBBBBBBBBBBBBBBBBBBBDRRRRRRHH",
    "HHBBBBBBBBBBBBBBBBBBBBBBBBBBBBBDDDDDDDDH",
    "HBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBDBBBBBBDH",
};
void draw_sprite(
    int worldX,
    int worldY,
    const char* sprite[],
    int h,
    int cameraLeftX,
    int cameraTopY
)
{
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; sprite[y][x] != '\0'; x++)
        {
            int sx = worldX - cameraLeftX + x;
            int sy = worldY - cameraTopY + y;

            char c = sprite[y][x];

            switch (c)
            {
            case 'R':
                draw_pixel(sx, sy, 200, 40, 40);
                break;

            case 'Y':
                draw_pixel(sx, sy, 230, 220, 120);
                break;

            case 'W':
                draw_pixel(sx, sy, 100, 180, 255);
                break;

            case 'D':
                draw_pixel(sx, sy, 120, 70, 20);
                break;

            case 'G':
                draw_pixel(sx, sy, 80, 200, 80);
                break;
            case 'C':
                draw_pixel(sx, sy, 255, 255, 255);
                break;
            case 'N':
                draw_pixel(sx, sy, 0, 0, 0);
                break;
            case 'Q':
                draw_pixel(sx, sy, 190, 190, 190);
                break;
            case 'B':
                draw_pixel(sx, sy, 165, 135, 100);
                break;
            case 'H':
                draw_pixel(sx, sy, 200, 200, 200);
                break;
            case 'J':
                draw_pixel(sx, sy, 210, 210, 210);
                break;
            }
        }
    }
}

int game_map() {
    printf("\033[?25l\033[2J");
    // 플레이어 초기 월드 좌표 (월드 중심 근처)
    int playerX = 13.0;
    int playerY = 26.0;
    int moveSpeed = 1; // GetAsyncKeyState를 위한 부드러운 이동 속도 조절

    // 카메라 좌상단 월드 좌표 (초기 계산용)
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

        // --- 1. GetAsyncKeyState를 이용한 부드러운 입력 처리 ---
        // 0x8000 	이전에 누른 적이 없고 호출 시점에서 눌린 상태
        if (GetAsyncKeyState('A') & 0x8000)  playerX -= moveSpeed;
        if (GetAsyncKeyState('D') & 0x8000) playerX += moveSpeed;
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) return 7; // ESC 누르면 종료

        // 플레이어 월드 경계선 체크 (0 ~ WORLD-1)
        if (playerX < 0) playerX = 0;
        if (playerX > WORLD_W - 1) playerX = WORLD_W - 1;
        if (playerY < 0) playerY = 0;
        if (playerY > WORLD_H - 1) playerY = WORLD_H - 1;

        // 정수형 플레이어 좌표 (렌더링 및 카메라 계산용)
        int px = (int)playerX;
        int py = (int)playerY;




        // 카메라 월드 경계선 예외 법칙 (카메라가 월드를 벗어나지 않게 고정)
        if (cameraLeftX < 0) cameraLeftX = 0;
        if (cameraLeftX > WORLD_W - SCR_W) cameraLeftX = WORLD_W - SCR_W;
        if (cameraTopY < 0) cameraTopY = 0;
        if (cameraTopY > WORLD_H - SCR_H) cameraTopY = WORLD_H - SCR_H;


        // --- 3. 렌더링 (그리기) ---

        // 플레이어가 실제로 이동했을 때만 잔상을 지우고 새로 그린다.
        // 이러면 매 프레임 그리지 않아도 되니까 화면 깜빡임(Flickering)이 확 줄어들어.
        if (oldPlayerX != playerX || oldPlayerY != playerY) {

            // [1단계] 지나간 옛날 자리(oldPlayerX, oldPlayerY) 완벽 복구하기
            char bg_char = 'W'; // 기본값은 파란 하늘

            // 1. SKY 배열에서 원래 타일 찾기 (구름, 땅 등)
            if (oldPlayerY >= 0 && oldPlayerY < 28 && oldPlayerX >= 0 && oldPlayerX < 40) {
                bg_char = SKY[oldPlayerY][oldPlayerX];
            }

            // 2. 만약 옛날 자리가 HOUSE(집) 영역이었다면 덮어쓰기
            // 집은 (1, 19)부터 너비 11, 높이 8
            if (oldPlayerX >= 1 && oldPlayerX < 1 + 11 && oldPlayerY >= 19 && oldPlayerY < 19 + 8) {
                char hc = HOUSE[oldPlayerY - 19][oldPlayerX - 1];
                if (hc != '.') bg_char = hc; // '.'은 투명한 공간이므로 무시
            }

            // 3. 만약 옛날 자리가 PHARMACY(약국) 영역이었다면 덮어쓰기
            // 약국은 (30, 19)부터 너비 9, 높이 8
            if (oldPlayerX >= 30 && oldPlayerX < 30 + 9 && oldPlayerY >= 19 && oldPlayerY < 19 + 8) {
                char pc = PHARMACY[oldPlayerY - 19][oldPlayerX - 30];
                if (pc != '.') bg_char = pc;
            }

            // 옛날 위치의 스크린 좌표 계산
            int old_sx = oldPlayerX - cameraLeftX;
            int old_sy = oldPlayerY - cameraTopY;


            switch (bg_char) {
            case 'W': draw_pixel(old_sx, old_sy, 100, 180, 255); break;
            }

            // [2단계] 새로운 위치에 플레이어 그리기
            int p_screen_x = playerX - cameraLeftX;
            int p_screen_y = playerY - cameraTopY;
            draw_pixel(p_screen_x, p_screen_y, 255, 255, 0);
        }
        // --- 4. 하단 UI 출력 (마지막 2줄 공간 활용) ---
        // 카메라의 중앙점 월드 좌표 계산
        int cameraCenterX = cameraLeftX + (SCR_W / 2);
        int cameraCenterY = cameraTopY + (SCR_H / 2);

        // UI 영역으로 커서 이동 (29번째 줄)
        printf("\033[%d;1H", CONSOLE_H - 1);
        // 줄 전체를 공백으로 밀어버려 글자 깨짐 방지
        //printf("\033[K[Camera Center World] X: %3d, Y: %3d\n", cameraCenterX, cameraCenterY);
        //printf("\033[K[Player World] X: %3d, Y: %3d (ESC:종료)",
         //   px, py);

        // 무한루프 방지 및 부드러운 프레임 유지를 위한 딜레이 
        Sleep(16);

        //위치출력
        printf("\033[%d;1H", SCR_H + 2);
        printf("================================");
        printf("\033[%d;1H", SCR_H + 3);
        printf("PLAYER POSITION : (%d, %d)     ", playerX, playerY);

        if (playerX == 10)
        {
            //집 이동
            printf("\033[?25h\033[2J\033[1;1H");
            return 5;
        }
        if (playerX == 30)
        {
            printf("\033[?25h\033[2J\033[1;1H");
            // 약국 내부로 이동
            return 6;

        }

    }

    // 종료 시 설정 복원 및 화면 청소
    printf("\033[?25h\033[2J\033[1;1H");
    return 5;
}


int house() {
    printf("\033[?25l\033[2J");
    mciSendString(TEXT("setaudio Room-bgm volume to 80"), NULL, 0, NULL);
    
    mciSendString(TEXT("play Room-bgm repeat"), NULL, 128, NULL);
    

    int cameraLeftX = 0;
    int cameraTopY = 0;
    int selectX = 1;
    int selectY = 1;

    draw_sprite(20, 5, ROOM, 25, cameraLeftX, cameraTopY);
    Move_Cursor(40, 30);
    printf("--------------------------------------------------------------------------------");
    while (1) {

            if (selectX == 1 && selectY == 1) {
                set_color(BG_COLOR_YELLOW);
                Move_Cursor(55, 34);
                printf("1.잠을 잔다 ");
                printf(COLOR_RESET);

                Move_Cursor(90, 34);
                printf("2.피아노를 연주한다 ");
                Move_Cursor (53, 36);
                printf("3.밖으로 나간다 ");
            }
            if (selectX == 2 && selectY == 1) {
                Move_Cursor(55, 34);
                printf("1.잠을 잔다 ");

                set_color(BG_COLOR_YELLOW);
                Move_Cursor(90, 34);
                printf("2.피아노를 연주한다 ");
                printf(COLOR_RESET);
                Move_Cursor(53, 36);
                printf("3.밖으로 나간다 ");
            }
            if (selectX == 1 && selectY == 2) {
                Move_Cursor(55, 34);
                printf("1.잠을 잔다 ");
                Move_Cursor(90, 34);
                printf("2.피아노를 연주한다 ");
                set_color(BG_COLOR_YELLOW);
                Move_Cursor(53, 36);
                printf("3.밖으로 나간다 ");
                printf(COLOR_RESET);
            }

            if (selectY != 1) {
                if (GetAsyncKeyState('W') & 0x8000)  selectY -= 1;
            }

            if (selectX != 1) {
                if (GetAsyncKeyState('A') & 0x8000) selectX -= 1;
            }
            if (selectY != 2) {
                if (GetAsyncKeyState('S') & 0x8000)  selectY += 1;
            }
            if (selectX != 2) {
                if (GetAsyncKeyState('D') & 0x8000) selectX += 1;
            }


            //잠을 잔다 미구현 
            /*
            if (selectX == 1 && selectY == 1) {
            if (GetAsyncKeyState(VK_RETURN) & 0x8000)

            }*/

            //피아노를 연주한다
            
            if (selectX == 2 && selectY == 1) {
                if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                    mciSendString(TEXT("pause Room-bgm"), NULL, 128, NULL);
                    mciSendString(TEXT("play piano"), NULL, 128, NULL);
                    Sleep(4700);
                    mciSendString(TEXT("seek piano to start"), NULL, 0, NULL);
                    mciSendString(TEXT("resume Room-bgm"), NULL, 128, NULL);
                }
             }
             //밖으로 나간다
            if (selectX == 1 && selectY == 2) {
                if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                    mciSendString(TEXT("stop Room-bgm"), NULL, 128, NULL);
                    mciSendString(TEXT("seek Room-bgm to start"), NULL, 0, NULL);
                    return 4;
                }
            }



        }
}
int game_Start() {

    //Sleep(2000);
    mciSendString(TEXT("close myBgm"), NULL, 0, NULL);
    int statues01 = "";

    system("cls");
    Move_Cursor(60, 25);
    printf("낮선 이곳에 이사오게된 당신\n");
    Sleep(1500);
    Move_Cursor(60, 26);
    printf("이곳에서 약국을 차리기로 하는데...");
    Sleep(4000);
    system("cls");
    Move_Cursor(60, 25);
    printf("???:맛있는거 내놔");
    Sleep(1500);
    Move_Cursor(60, 26);
    printf("네?");
    Sleep(3000);
    system("cls");
    Move_Cursor(60, 25);
    printf("아무래도 잘못 개업한거 같다");

    Sleep(4000);
   // while (statues01 != 27) {
   //     statues01 = getch();
        // 머하지...
    //}
    system("cls");
    return 5; // return 5;
}

int main_game()
{
    int day = 1;
    int money = 10000;
    int satisfaction = 50;
    int customer_count = 0;

    char medicine[50];

    typedef struct
    {
        char dialogue[200];
        char answer[50];
    } Customer;

    srand(time(NULL));

    while (day <= 10)
    {
        system("cls");

        printf("=================================\n");
        printf("        약국 시뮬레이터\n");
        printf("=================================\n");
        printf("현재 날짜 : %d일차\n", day);
        printf("돈 : %d원\n", money);
        printf("만족도 : %d\n", satisfaction);
        printf("\n");

        printf("[대기중...]\n");

        printf("엔터를 누르면 손님 등장\n");

        (void)getchar();

        Customer customers[10] =
        {
            {"손님1 대사", "약1"},
            {"손님2 대사", "약2"},
            {"손님3 대사", "약3"},
            {"손님4 대사", "약4"},
            {"손님5 대사", "약5"},
            {"손님6 대사", "약6"},
            {"손님7 대사", "약7"},
            {"손님8 대사", "약8"},
            {"손님9 대사", "약9"},
            {"손님10 대사", "약10"}
        };

        int customerIndex = rand() % 10;

        Customer customer = customers[customerIndex];

        printf("\n");
        printf("손님 등장!\n");
        printf("\n");

        char* goodLines[10] = // 긍정대사
        {
            "긍정대사1",
            "긍정대사2",
            "긍정대사3",
            "긍정대사4",
            "긍정대사5",
            "긍정대사6",
            "긍정대사7",
            "긍정대사8",
            "긍정대사9",
            "긍정대사10"
        };

        char* badLines[10] = //부정대사
        {
            "부정대사1",
            "부정대사2",
            "부정대사3",
            "부정대사4",
            "부정대사5",
            "부정대사6",
            "부정대사7",
            "부정대사8",
            "부정대사9",
            "부정대사10"
        };

        printf("%s\n", customer.dialogue);

        //--------------------------------
        // 약 선택
        //--------------------------------

        printf("\n");
        printf("판매할 약 이름 입력 : ");
         
        scanf_s("%49s", medicine, (unsigned)_countof(medicine));

        //--------------------------------
        // 판정
        //--------------------------------

        if (strcmp(medicine, customer.answer) == 0)
        {
            int line = rand() % 10;

            printf("\n");
            printf("%s\n", goodLines[line]);

            money += 5000;
            satisfaction += 5;
        }
        else
        {
            int line = rand() % 10;

            printf("\n");
            printf("%s\n", badLines[line]);

            money -= 3000;
            satisfaction -= 5;
        }

        //--------------------------------
        // 결과
        //--------------------------------

        printf("\n");
        printf("현재 돈 : %d\n", money);
        printf("현재 만족도 : %d\n", satisfaction);

        printf("\n손님이 떠났습니다.\n");

        printf("\n계속하려면 엔터...");

        (void)getchar();
        (void)getchar();


        customer_count++;

        if (customer_count == 4)
        {
            day++;
        }
    }
}

int credit_Scr() {
    int statues01 = "";

    system("cls");
    Move_Cursor(50, 5);
    printf("만든 놈들!!!!!\n");
    //Sleep(4000);
    Move_Cursor(50, 16);
    printf("킹왕짱 멋진 조장 202619005 이현진\n");
    Move_Cursor(50, 17);
    printf("개발, 아트 따까리1 202617641 강보민\n");
    Move_Cursor(50, 18);
    printf("개발, 아트 따까리2 202617100 허규빈\n");

    Move_Cursor(50, 20);
    printf("우정 출연:\n");
    Move_Cursor(50, 21);
    printf("AI콘텐츠공학과 이주영, 기계융합학부 1, 에피드게임즈 게임 \"트릭컬\" 스피키, 에르핀..");
    Move_Cursor(50, 23);
    printf("돌아가시려면 esc키를 누르십시오");
    while (statues01 != 27) {
        statues01 = getch();
        // 머하지...
    }
    system("cls");
    return 0; // return 5;
}

int loading() {
    //게임 로고 등장

    system("cls");
    Move_Cursor(10, 5);
    printf("본 게임은 전체화면으로 플레이하는 것을 전제로 제작 되었습니다");
    Move_Cursor(10, 6);
    printf("전체화면으로 전환해 주세요");
    Sleep(4000);

    system("cls");

    FILE* logo;
    char line[1024]; // 한 줄을 저장할 버퍼 (길이에 따라 조절 가능)

    // 파일 열기
    logo = fopen("logo.txt", "r");
    mciSendString(TEXT("open \"mp3s/nerujimaseyo.mp3\" type mpegvideo alias logo"), NULL, 0, NULL);
    mciSendString(TEXT("open \"mp3s/voice 7 voice.mp3\" type mpegvideo alias myBgm"), NULL, 0, NULL);
    mciSendString(TEXT("open \"mp3s/Room-bgm.mp3\" type mpegvideo alias Room-bgm"), NULL, 0, NULL);
    mciSendString(TEXT("open \"mp3s/Piano Sound Effect.mp3\" type mpegvideo alias piano"), NULL, 0, NULL);
    // 파일 끝까지 한 줄씩 읽어서 출력
    while (fgets(line, sizeof(line), logo) != NULL) {
        printf("%s", line);
    }

    mciSendString(TEXT("play logo repeat"), NULL, 0, NULL);
    mciSendString(TEXT("play logo wait"), NULL, 0, NULL);
    // 파일 닫기
    fclose(logo);
    // 1. 배경음악
    mciSendString(TEXT("setaudio myBgm volume to 100"), NULL, 0, NULL);
    mciSendString(TEXT("play myBgm repeat"), NULL, 0, NULL);
    mciSendString(TEXT("close logo"), NULL, 0, NULL);
    //mciSendString(TEXT("close myBgm"), NULL, 0, NULL);

    return 0;

}

int render_Title() {
    int posY = 25;
    int color_num = 1;
    int garo;
    int sero;
    int select = 1;

    char input = "";
    CONSOLE_SCREEN_BUFFER_INFO gasebi;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &gasebi);
    garo = gasebi.srWindow.Right - gasebi.srWindow.Left;
    sero = gasebi.srWindow.Bottom - gasebi.srWindow.Top;


    Sleep(1000);
    //Move_Cursor(garo / 2, sero / 2 - 4);
    //printf("로딩중입니다람쥐");


    while (input != 27) {



        system("cls");
        //printf(color_num);
        Move_Cursor(garo / 2, sero / 2 - 4);
        set_color(BG_COLOR_BrRED);
        printf("엘리아스의 약국");
        set_color(BG_COLOR_BLACK);

        if (select == 1) {
            set_color(BG_COLOR_YELLOW);
            Move_Cursor(garo / 2, sero / 2 - 3);
            printf("game start");
            printf(COLOR_RESET);
            Move_Cursor(garo / 2, sero / 2 - 1);
            printf("credit");
            Move_Cursor(garo / 2, sero / 2 + 1);
            printf("exit");
        }


        if (select == 2) {
            Move_Cursor(garo / 2, sero / 2 - 3);
            printf("game start");
            set_color(BG_COLOR_YELLOW);
            Move_Cursor(garo / 2, sero / 2 - 1);
            printf("credit");
            printf(COLOR_RESET);
            Move_Cursor(garo / 2, sero / 2 + 1);
            printf("exit");
        }

        if (select == 3) {
            Move_Cursor(garo / 2, sero / 2 - 3);
            printf("game start");
            Move_Cursor(garo / 2, sero / 2 - 1);
            printf("credit");
            set_color(BG_COLOR_YELLOW);
            Move_Cursor(garo / 2, sero / 2 + 1);
            printf("exit");
            printf(COLOR_RESET);
        }

        Move_Cursor(0, 0);
        printf("%d", select);

        /*
        if (color_num == 1) {
            set_color(BG_COLOR_YELLOW);
            Move_Cursor(garo / 2, sero / 2 - 3);
            printf("game start");
            printf(COLOR_RESET);
            Move_Cursor(garo / 2, sero /2 +1);
            printf("exit");
            Move_Cursor(garo /2 , sero /2 -1);
            printf("credit");
        }

        if (color_num == 2) {
            set_color(BG_COLOR_YELLOW);
            Move_Cursor(garo / 2, sero / 2 - 3);
            printf("game start");
            printf(COLOR_RESET);
            Move_Cursor(garo / 2, sero / 2 + 1);
            printf("exit");
            Move_Cursor(garo / 2, sero / 2 - 1);
            printf("credit");
        }

        if (color_num == 3) {
            set_color(BG_COLOR_YELLOW);
            Move_Cursor(garo / 2, sero / 2 - 3);
            printf("game start");
            printf(COLOR_RESET);
            Move_Cursor(garo / 2, sero / 2 + 1);
            printf("exit");
            Move_Cursor(garo / 2, sero / 2 - 1);
            printf("credit");
        }
        */


        //Move_Cursor((garo + 10) / 2, ));
        //printf("<==");

        input = getch();

        switch (input) {
        case 'w':
            if (select > 1) {
                select = select - 1;

            }
            break;
        case 's':
            if (select < 3) {
                select = select + 1;

            }
            break;
        case 13:
            if (select == 1) {
                return 1;
                break;
            }
            if (select == 2) {
                return 3;
                break;
            }
        case 27:
            return 7;
            break;
        }



        /*
        switch (input) {
        case 'w':
            if (posY > 25) {
                posY = posY - 2;
                color_num = color_num - 1;
            }
            break;
        case 's':
            if (posY < 29) {
                posY = posY + 2;
                color_num = color_num + 1;
            }
            break;
        case 13:
            if (color_num == 1) {
                return 1;
                break;
            }
            if (color_num == 3) {
                return 3;
                break;
            }
        case 27:
            return 5;
            break;
        }
        */

        //system("cls");
    }

    return 0;
}


int main() {
    int selec_Op = -1;
    SetConsoleOutputCP(CP_UTF8);


    while (1)
    {
        switch (selec_Op)
        {
        case -1:
            selec_Op = loading();
            break;
        case 0:
        gameBack:
            selec_Op = render_Title();
            break;
        case 1:
            selec_Op = game_Start();
            break;
        case 3:
            selec_Op = credit_Scr();
            break;
        case 4:
            selec_Op = game_map();
            break;
        case 5:
            selec_Op = house();
            break;
        case 6:
            selec_Op = main_game();
            break;
        case 7:
            goto exit;
        }

    }

exit:
    system("cls");
    printf("게임을 종료하시겠습니까?\n");
    printf("취소하려면 esc키를, 종료하려면 엔터키를 입력하십시오");
    int exit_key01 = getch();
    if (exit_key01 == 13) {
        Move_Cursor(0, 21);
        return printf("break"); // 실행 종료시 break 문자 출력
    }
    if (exit_key01 == 27)
    {
        goto gameBack;
    }
}



