#include <stdio.h>
#include <conio.h>
#include <windows.h>
#define WIDTH 90
#define HEIGHT 150

void GetInput();
void Setup();
void SummonBlock();
void MoveSand();
void Draw();


int save[HEIGHT][WIDTH];
int lastSave[HEIGHT][WIDTH];
char w[HEIGHT*(WIDTH + 1)];


int main(){
    Setup();
    while (1){
        SummonBlock();
        GetInput();
        MoveSand();
        Draw();
    }
    return 0;
}


void GetInput(){
    char key;
    if (_kbhit() != 0){
        key = _getch();
        if (key == 75){
            for (int i = 0; i < HEIGHT; i++)
            for (int j = 1; j < WIDTH; j++)
                if (save[i][j] == -1){
                    save[i][j - 1] = save[i][j];
                    save[i][j] = 0;
                }
        } else if (key == 77){
            for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIDTH - 1; j++)
            if (save[i][j] == -1){
                save[i][j + 1] = save[i][j];
                save[i][j] = 0;
            }
        }
    }
}


void Draw(){
    // system("clear");
    // for (int i = 0; i < HEIGHT; i++){
    //     for (int j = 0; j < WIDTH; j++){
    //         system("cls");
    //     }
    //     system("cls");
    // }


    // system("cls");
    // // COORD pos = {0, 0};
    // // SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    // // printf("\0");
    // for (int i = 0; i < HEIGHT * (WIDTH + 1); i++){
    //     w[i] = i % (WIDTH + 1) == WIDTH ? '\n' : save[i / (WIDTH + 1)][i % (WIDTH + 1)] == 0 ? 'X' : 'O';
    // }
    // printf("%s", w);

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            if (lastSave[i][j] != save[i][j]){
                COORD pos;
                pos.X = j;
                pos.Y = i;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
                printf("%c", save[i][j] == 0 ? 'X' : 'O');
                lastSave[i][j] = save[i][j];
            }
            // Sleep(100);
        }
    }
    // for (int i = 0; i < HEIGHT * WIDTH; i++){
    //     if (lastSave[i / WIDTH][i % WIDTH] != save[i / WIDTH][i % WIDTH]){
    //         COORD pos = {i / WIDTH, i % WIDTH};
    //         SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    //         printf("\b");
    //         printf("%c", save[i / WIDTH][i % WIDTH] == 1 ? 'O' : 'X');
    //     }

    // }
    // printf("%s", w);


    // for (int i = 0; i < HEIGHT; i++){
    //     for (int j = 0; j < WIDTH; j++){
    //         COORD pos = {0, 0};
    //         SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
    //         printf("\b");
    //         printf("%c", save[i][j]==1?'O':'X');
    //     }
    //     printf("\n");
    // }
}


void Setup(){
    system("mode con cols=100 lines=160 | title test");
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            save[i][j] = 0;
            lastSave[i][j] = 0;
            COORD pos;
            pos.X = j;
            pos.Y = i;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
            printf("%c", save[i][j] == 0?'X':'O');
        }
        COORD pos;
        pos.X = WIDTH;
        pos.Y = i;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        printf("\n");
    }
}

void SummonBlock(){
    save[0][WIDTH/2] = 1;
}

void MoveSand(){
    for (int i = HEIGHT - 2; i >= 0; i--){
        for (int j = 0; j < WIDTH; j++){
            if (save[i][j] != 0){
                if (save[i + 1][j] == 0){
                    save[i + 1][j] = save[i][j];
                    save[i][j] = 0;
                }else{
                    save[i][j] = -1;
                    if(j > 0 && save[i + 1][j - 1] == 0){
                        save[i + 1][j - 1] = save[i][j];
                        save[i][j] = 0;
                    }else if(j < WIDTH - 1 && save[i + 1][j + 1] == 0){
                        save[i + 1][j + 1] = save[i][j];
                        save[i][j] = 0;
                    }
                }

            }

        }
    }
}


