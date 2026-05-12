#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> // stdio.h 파일 가져오기
#include <windows.h> // windows.h 파일 가져오기
#pragma execution_character_set("utf-8")
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib") // MSVC 컴파일러용


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



int game_Start() {

    Sleep(2000);
    mciSendString(TEXT("close myBgm"), NULL, 0, NULL);
    int statues01 = "";

    system("cls");
    Move_Cursor(60, 15);
    printf("Once upon a time....\n");
    //Sleep(4000);
    Move_Cursor(60, 16);
    printf("Someone has appeared who was called 케인");
    Move_Cursor(60, 17);
    printf("enter");

    while (statues01 != 27) {
        statues01 = getch();
        // 머하지...
    }
    return 0; // return 5;
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

    FILE* fp;
    char line[1024]; // 한 줄을 저장할 버퍼 (길이에 따라 조절 가능)

    // 파일 열기
    fp = fopen("logo.txt", "r");
    mciSendString(TEXT("open \"nerujimaseyo.mp3\" type mpegvideo alias logo"), NULL, 0, NULL);
    mciSendString(TEXT("open \"voice 7 voice.mp3\" type mpegvideo alias myBgm"), NULL, 0, NULL);


    // 파일 끝까지 한 줄씩 읽어서 출력
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }

    mciSendString(TEXT("play logo repeat"), NULL, 0, NULL);
    mciSendString(TEXT("play logo wait"), NULL, 0, NULL);
    // 파일 닫기
    fclose(fp);
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
    char input = "";

    Sleep(1000);


 
    while (input != 27) {

        system("cls");
        //printf(color_num);
        Move_Cursor(135, 20);
        set_color(BG_COLOR_BrRED);
        printf("약국왕");
        set_color(BG_COLOR_BLACK);

        if (color_num == 1) {
            set_color(BG_COLOR_YELLOW);
            Move_Cursor(130, 25);
            printf("game start");
            printf(COLOR_RESET);
            Move_Cursor(130, 27);
            printf("exit");
            Move_Cursor(130, 29);
            printf("credit");
        }

        if (color_num == 2) {
            Move_Cursor(130, 25);
            printf("game start");
            set_color(BG_COLOR_YELLOW);
            Move_Cursor(130, 27);
            printf("exit");
            printf(COLOR_RESET);
            Move_Cursor(130, 29);
            printf("credit");
        }

        if (color_num == 3) {
            Move_Cursor(130, 25);
            printf("game start");
            Move_Cursor(130, 27);
            printf("exit");
            set_color(BG_COLOR_YELLOW);
            Move_Cursor(130, 29);
            printf("credit");
            printf(COLOR_RESET);
        }

        Move_Cursor(145, posY);
        printf("<==");

        input = getch();
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



