#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> // stdio.h 파일 가져오기
#include <windows.h> // windows.h 파일 가져오기
#pragma execution_character_set("utf-8")
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib") // MSVC 컴파일러용


// 월드 좌표 지정
#define SCR_W 40  // X좌표 2칸이 1칸이므로 80 / 2 = 40
#define SCR_H 28  // 하단 UI 2줄을 제외한 30 - 2 = 28

// 월드 크기 정의
#define WORLD_W 40
#define WORLD_H 28
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

int game_map() {
        printf("\033[?25l\033[2J");
        // 플레이어 초기 월드 좌표 (월드 중심 근처)
        double playerX = 50.0;
        double playerY = 50.0;
        double moveSpeed = 0.8; // GetAsyncKeyState를 위한 부드러운 이동 속도 조절

        // 카메라 좌상단 월드 좌표 (초기 계산용)
        int cameraLeftX = 0;
        int cameraTopY = 0;

        while (1) {
            // --- 1. GetAsyncKeyState를 이용한 부드러운 입력 처리 ---
            // 0x8000 	이전에 누른 적이 없고 호출 시점에서 눌린 상태
            if (GetAsyncKeyState('A') & 0x8000)  playerX -= moveSpeed;
            if (GetAsyncKeyState('D') & 0x8000) playerX += moveSpeed;
            if (GetAsyncKeyState('W') & 0x8000)    playerY -= moveSpeed;
            if (GetAsyncKeyState('S') & 0x8000)  playerY += moveSpeed;
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) break; // ESC 누르면 종료

            // 플레이어 월드 경계선 체크 (0 ~ WORLD-1)
            if (playerX < 0) playerX = 0;
            if (playerX > WORLD_W - 1) playerX = WORLD_W - 1;
            if (playerY < 0) playerY = 0;
            if (playerY > WORLD_H - 1) playerY = WORLD_H - 1;

            // 정수형 플레이어 좌표 (렌더링 및 카메라 계산용)
            int px = (int)playerX;
            int py = (int)playerY;


            // --- 2. 카메라 위치 계산 (1/3 ~ 2/3 영역 유지 및 예외 처리) ---
            int limitLeft = SCR_W / 3;
            int limitRight = (SCR_W * 2) / 3;
            int limitTop = SCR_H / 3;
            int limitBottom = (SCR_H * 2) / 3;

            // 현재 카메라 기준 플레이어의 상대 스크린 좌표
            int sX = px - cameraLeftX;
            int sY = py - cameraTopY;

            // X축 카메라 추적
            if (sX < limitLeft)       cameraLeftX = px - limitLeft;
            else if (sX > limitRight) cameraLeftX = px - limitRight;

            // Y축 카메라 추적
            if (sY < limitTop)        cameraTopY = py - limitTop;
            else if (sY > limitBottom) cameraTopY = py - limitBottom;

            // 카메라 월드 경계선 예외 법칙 (카메라가 월드를 벗어나지 않게 고정)
            if (cameraLeftX < 0) cameraLeftX = 0;
            if (cameraLeftX > WORLD_W - SCR_W) cameraLeftX = WORLD_W - SCR_W;
            if (cameraTopY < 0) cameraTopY = 0;
            if (cameraTopY > WORLD_H - SCR_H) cameraTopY = WORLD_H - SCR_H;


            // --- 3. 렌더링 (그리기) ---

            // 더블버퍼링이 없으므로 잔상을 지우기 위해 커서를 상단으로 올려 덮어쓰기 유도
            printf("\033[1;1H");


            
            
            // 배경 및 5픽셀 단위 바둑판 그리드 그리기
            for (int y = 0; y < SCR_H; y++) {
                int wy = cameraTopY + y; // 현재 스크린 y에 해당하는 월드 y 좌표

                for (int x = 0; x < SCR_W; x++) {
                    int wx = cameraLeftX + x; // 현재 스크린 x에 해당하는 월드 x 좌표

                    // 5픽셀 단위 바둑판 모양 격자선 검사 (5로 나누어 떨어질 때 회색 선)
                    if (wx % 5 == 0 || wy % 5 == 0) {
                        draw_pixel(x, y, 80, 80, 80); // 회색 선
                    }
                    else {
                        draw_pixel(x, y, 15, 15, 30); // 짙은 네이비 배경
                    }
                }
            }
            


            // 플레이어 화면 좌표 계산 후 노란색(255, 255, 0)으로 그리기
            int p_screen_x = px - cameraLeftX;
            int p_screen_y = py - cameraTopY;
            draw_pixel(p_screen_x, p_screen_y, 255, 255, 0);


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
        }

        // 종료 시 설정 복원 및 화면 청소
        printf("\033[?25h\033[2J\033[1;1H");
        return 5;
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
    return 4; // return 5;
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
            return 5;
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
        case 5:
            goto exit; // 게임 종료
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



