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
void DestroySand();
void CheckCollision();


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
                if(save[i][j] == '0') 
            		printf(" ");
            	else
            		printf("ㅁ");
                //printf("%s", save[i][j] == 0 ? " " : "■");
                //printf("%d", save[i][j]);
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
            if(save[i][j] == '0') 
            	printf(" ");
            else
            	printf("ㅁ");
            //printf("%s", save[i][j] == 0?" ":"ㅁ");
            //printf("%d", save[i][j]);
        }
        COORD pos;
        pos.X = WIDTH;
        pos.Y = i;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        printf("\n");
    }
}

void SummonBlock(){ // 0: 빈 공간 1: 조작할 수 없는 픽셀 // -1조작 가능한 픽셀  
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

void DestorySand(){
	int visited[HEIGHT][WIDTH]={0,};
	for(int l=0;l<HEIGHT;++l){
		if(visited[l][0]||save[l][0]<1) continue;
		int queue[HEIGHT*WIDTH]={0,},qsize=1,yesclear=0;
		queue[0]=l*HEIGHT;
		visited[l][0]=l+1;
		// look for same color mong-tang-ee
		for(int i=0;i<qsize;++i){
			yesclear=((queue[i]+1)%WIDTH<1)|yesclear;
			for(int j=0;j<4;++j){
				int cx=queue[i]/HEIGHT+"1210"[j]-'1', cy=queue[i]%HEIGHT+"0121"[j]-'1';
				if(cx<0||cy<0||cx>=HEIGHT||cy>=WIDTH||visited[cx][cy]||save[l][0]!=save[cx][cy]) continue;
				visited[cx][cy]=l+1;
				queue[qsize++]=cx*HEIGHT+cy;
			}
		}
		// check if the line is erasable
		if(!yesclear) continue;
		// destroy sand
		qsize=1;
		queue[0]=l*HEIGHT;
		visited[l][0]=0;
		for(int i=0;i<qsize;++i){
			// insert Score UP function here
			save[queue[i]/HEIGHT][queue[i]%HEIGHT]=0;
			for(int j=0;j<4;++j){
				int cx=queue[i]/HEIGHT+"1210"[j]-'1', cy=queue[i]%HEIGHT+"0121"[j]-'1';
				if(cx<0||cy<0||cx>=HEIGHT||cy>=WIDTH||visited[cx][cy]!=l+1) continue;
				visited[cx][cy]=0;
				queue[qsize++]=cx*HEIGHT+cy;
			}
		}
	}
}

void CheckCollision(){
	int visited[HEIGHT][WIDTH]={0,};
	for(int i=0;i<HEIGHT;++i){
		for(int j=0;j<WIDTH;++j){
			visited[i][j]=1;
			if(save[i][j]>=0) continue;
			int queue[HEIGHT*WIDTH]={0,},qsize=1,yescollision=0;
			queue[0]=i*HEIGHT+j;
			for(int k=0;k<qsize;++k){
				yescollision=yescollision|(queue[k]/HEIGHT==HEIGHT-1);
				for(int v=0;v<4;++v){
					int cx=queue[k]/HEIGHT+"1210"[v]-'1', cy=queue[k]%HEIGHT+"0121"[v]-'1';
					if(cx<0||cy<0||cx>=HEIGHT||cy>=WIDTH) continue;
					if(cx-1==queue[i]/HEIGHT&&save[cx][cy]>0) yescollision=1;
					if(save[cx][cy]!=save[i][j]||visited[cx][cy]) continue;
					visited[cx][cy]=1;
					queue[qsize++]=cx*HEIGHT+cy;
				}
			}if(!yescollision) return;
			qsize=1;
			queue[0]=i*HEIGHT+j;
			save[i][j]=-save[i][j];
			for(int k=0;k<qsize;++k){
				for(int v=0;v<4;++v){
					int cx=queue[k]/HEIGHT+"1210"[v]-'1', cy=queue[k]%HEIGHT+"0121"[v]-'1';
					if(cx<0||cy<0||cx>=HEIGHT||cy>=WIDTH||save[cx][cy]!=save[i][j]) continue;
					save[cx][cy]=-save[cx][cy];
					queue[qsize++]=cx*HEIGHT+cy;
				}
			}SummonBlock();
			return;
		}
	}
}
