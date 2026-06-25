#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#define WIDTH 90
#define HEIGHT 150

void GetInput(); // 입력을 받고, 입력에 맞는 함수를 실행합니다. 
void Setup(); // 처음 UI를 프린트합니다. 
void SummonBlock(); // 블록을 생성합니다. 
void MoveSand(); // 모래 물리 함수입니다. 
void Draw(); // 바뀐 모래를 그립니다. 
void DestroySand(); // 가로로 연속하는 모래를 파괴합니다. 
void SwitchBlock(); // 저장된 모래와 현재 모래 블록을 바꿉니다. 
void CheckCollision(); // 모래 블록이 충돌했는지 확인하고, 다음 모래 블록을 생성합니다. 
void Summon(char[][4][4], int); // 모래 생성 헬퍼 함수입니다. 
void Rotate(); // 모래 회전 함수입니다. 
void UpdateNext(); // 다음 모래 UI를 업데이트합니다. 
void printText(int text, int x, int y); // UI 헬퍼 함수입니다. 
void SaveBlock(); // 블록 저장 함수입니다. 


int save[HEIGHT][WIDTH];
int lastSave[HEIGHT][WIDTH];
char w[HEIGHT*(WIDTH + 1)];


char IBlock[2][4][4] =
{{{0, 0, 1, 0},
 {0, 0, 1, 0},
 {0, 0, 1, 0},
 {0, 0, 1, 0}},

 {{0, 0, 0, 0},
 {1, 1, 1, 1},
 {0, 0, 0, 0},
 {0, 0, 0, 0}}};

char OBlock[1][4][4] =
{{{0, 0, 0, 0},
 {0, 1, 1, 0},
 {0, 1, 1, 0},
 {0, 0, 0, 0}}};


char TBlock[4][4][4] =
{{{0, 0, 0, 0},
 {0, 1, 1, 1},
 {0, 0, 1, 0},
 {0, 0, 0, 0}},
 {{0, 0, 1, 0},
 {0, 0, 1, 1},
 {0, 0, 1, 0},
 {0, 0, 0, 0}},
 {{0, 0, 1, 0},
 {0, 1, 1, 1},
 {0, 0, 0, 0},
 {0, 0, 0, 0}},
 {{0, 0, 1, 0},
 {0, 1, 1, 0},
 {0, 0, 1, 0},
 {0, 0, 0, 0}}};


char SBlock[2][4][4] =
{{{0, 0, 0, 0},
 {0, 0, 1, 1},
 {0, 1, 1, 0},
 {0, 0, 0, 0}},
 {{0, 0, 1, 0},
 {0, 0, 1, 1},
 {0, 0, 0, 1},
 {0, 0, 0, 0}}};


char ZBlock[2][4][4] =
{{{0, 0, 0, 0},
 {0, 1, 1, 0},
 {0, 0, 1, 1},
 {0, 0, 0, 0}},
 {{0, 0, 0, 1},
 {0, 0, 1, 1},
 {0, 0, 1, 0},
 {0, 0, 0, 0}}};

 
char LBlock[4][4][4] =
{{{0, 0, 0, 0},
 {0, 1, 1, 1},
 {0, 1, 0, 0},
 {0, 0, 0, 0}},
 {{0, 0, 1, 0},
 {0, 0, 1, 0},
 {0, 0, 1, 1},
 {0, 0, 0, 0}},
 {{0, 0, 0, 1},
 {0, 1, 1, 1},
 {0, 0, 0, 0},
 {0, 0, 0, 0}},
 {{0, 1, 1, 0},
 {0, 0, 1, 0},
 {0, 0, 1, 0},
 {0, 0, 0, 0}}};

char JBlock[4][4][4] =
{{{0, 0, 0, 0},
 {0, 1, 1, 1},
 {0, 0, 0, 1},
 {0, 0, 0, 0}},
 {{0, 0, 1, 1},
 {0, 0, 1, 0},
 {0, 0, 1, 0},
 {0, 0, 0, 0}},
 {{0, 1, 0, 0},
 {0, 1, 1, 1},
 {0, 0, 0, 0},
 {0, 0, 0, 0}},
 {{0, 0, 1, 0},
 {0, 0, 1, 0},
 {0, 1, 1, 0},
 {0, 0, 0, 0}}};

char (*currentBlock)[4][4];

//Color, Type
int nextBlockInfo[2] = {};

int score = 0;


char (*savedBlock)[4][4];
int savedColor;
int isSaved = 0;
int canSave = 1;
int savedRotateNum;

int rotateNum;
int totRotateNum;
int currentBlockColor;
int blockIdx[2] = {WIDTH / 2 - 18, 0};

int fallCounter = 0;
int fallCounterOffset = 2;
int fastFall = 0;

int ended = 0;

void gotoxy(int x, int y){
	COORD pos = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

char FONT[16][7][100] = {
	{
		"  ___  ",
		" / _ \\ ",
		"| | | |",
		"| | | |",
		"| |_| |",
		" \\___/ "
	},
	{
		"  __ ",
		" /_ |",
		"  | |",
		"  | |",
		"  | |",
		"  |_|"
	},
	{
		" ___   ",
		"|__ \\ ",
		"   ) | ",
		"  / /  ",
		" / /_  ",
		"|____| "
	},
	{	
		" ____  ",
		"|___ \\ ",
		"  __) |",
		" |__ < ",
		" ___) |",
		"|____/ "
	},
	{	
		" _  _   ",
		"| || |  ",
		"| || |_ ",
		"|__   _|",
		"   | |  ",
		"   |_|  "
	},
	{
		" _____ ",
		"| ____|",
		"| |__  ",
		"|___ \\",
		" ___) |",
		"|____/ "
	},
	{
		"   __  ",
		"  / /  ",
		" / /_  ",
		"| '_ \\ ",
		"| (_) |",
		" \\___/ "
	},	
	{
		" ______ ",
		"|____  |",
		"    / / ",
		"   / /  ",
		"  / /   ",
		" /_/    "
	},
	{
		"  ___  ",
		" / _ \\ ",
		"| (_) |",
		" > _ < ",
		"| (_) |",
		" \\___/ "
	},
	{	
		"  ___  ",
		" / _ \\ ",
		"| (_) |",
		" \\__, |",
		"   / / ",
		"  /_/  "
	},
	
	{ 
		" _   _ ________   _________ ", 
		"| \\ | |  ____\\ \\ / /__   __|", 
		"|  \\| | |__   \\ V /   | |   ",
		"| . ` |  __|   > <    | |   ", 
		"| |\\  | |____ / . \\   | |   ", 
		"|_| \\_|______/_/ \\_\\  |_|   "
	},
	{
		" ____          _____ ",
		"|  _ \\   /\\   / ____|",
		"| |_) | /  \\ | |  __ ",
		"|  _ < / /\\ \\| | |_ |",
		"| |_) / ____ \\ |__| |",
		"|____/_/    \\_\\_____|"
	},
	{
		"  _____  _____ ____  _____  ______ ",
		" / ____|/ ____/ __ \\|  __ \\|  ____|",
		"| (___ | |   | |  | | |__) | |__   ",
		" \\___ \\| |   | |  | |  _  /|  __|  ",
		" ____) | |___| |__| | | \\ \\| |____ ",
		"|_____/ \\_____\\____/|_|  \\_\\______|"
	},
	{
		" _______ _____ __  __ ______ ",
		"|__   __|_   _|  \\/  |  ____|",
		"   | |    | | | \\  / | |__   ",
		"   | |    | | | |\\/| |  __|  ",
		"   | |   _| |_| |  | | |____ ",
		"   |_|  |_____|_|  |_|______|"
	},
	{
		" _      _____ _   _ ______  _____ ",
		"| |    |_   _| \\ | |  ____|/ ____|",
		"| |      | | |  \\| | |__  | (___  ",
		"| |      | | | . ` |  __|  \\___ \\ ",
		"| |____ _| |_| |\\  | |____ ____) |",
		"|______|_____|_| \\_|______|_____/ "
		
	},
	{
		"   ",
		" _ ",
		"(_)",
		"   ",
		"   ",
		"(_)",
		"    "
	}
};


int lt_pos[2] = {111, 0};

void printBoard() {
	
	gotoxy(95,0);
	for(int i = 0; i <= 159; i++) {
		
		gotoxy(95,0+i);
		if(i >= 150) {
			printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
		}
		else {
			printf("■■■■■■■■");
			for(int j = 0; j < 5; j++)
				printf("                  ");
			printf("■■■■■■■■");
		}

	}
    // NEXT, 136 - 36 = 100번째부터, 54 - 36 = 18번째부터
    //233, 36
		printText(10,225,30);
	gotoxy(219,33);
	printf("■■■");
	gotoxy(250,33);
	printf("■■■■■■■■■■■■■■■■■■■■■■");	
	for(int i = 0; i < 42; i++) {
		gotoxy(219,34+i);
		printf("■");
		gotoxy(292,34+i);
		printf("■");
	}
	gotoxy(219,75);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");	
	// BAG, 35번째부터, 36번째부터
	printText(11,22,30);
	gotoxy(16,33);
	printf("■■■");
	gotoxy(41,33);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■");	
	for(int i = 0; i < 42; i++) {
		gotoxy(16,34+i);
		printf("■");
		gotoxy(89,34+i);
		printf("■");
	}
	gotoxy(16,75);
	printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	printText(12,225, 90);
	printText(13,225, 120);
	printText(14,225, 150);
	gotoxy(225,600);

	for(int i = 0; i < 22; i++) {
		gotoxy(310,i+89);
		printf("■");
	}
	for(int i = 0; i < 12; i++) {
		gotoxy(225, i + 100);
		printf("■");
	}

	
	
}
void printNums(int score, int x, int y) {
	int cnt = 0;
	if(score == 0) {
		printText(0,x,y);
		return;
	}
	while(score >= 1) {
		printText(score % 10, x - cnt * 6, y);
		score /= 10;
		cnt += 1;
	}
}

void printText(int text, int x, int y) {
	gotoxy(x, y);
	for(int i = 0; i < 6; i++) {
		printf("%s", FONT[text][i]);
		gotoxy(x,y+i+1);
	}
}


int main(){
    Setup();
    SummonBlock();
    UpdateNext();
    while (!ended){
        fastFall = 0;
        // SummonBlock();
        if (fallCounter == 0){
            fallCounter = 2;
            MoveSand();
        }
        GetInput();
		CheckCollision();
        if (ended) break;
		DestroySand();
        Draw();
       	//if (!fastFall) Sleep(20);
        fallCounter --;
    }
    while (1){
        if (_kbhit() != 0){
            char key = _getch();
            if (key == 32) return 0;
        }

    }
    // return 0;
}


void GetInput(){
    char key;
    if (_kbhit() != 0){
        key = _getch();
        int flag = 1;
        if (key == 75){
            for (int i = 0; i < HEIGHT; i++)
            if (save[i][0] < 0){
                flag = 0;
                break;
            }
            if (flag){
                for (int i = 0; i < HEIGHT; i++)
                for (int j = 1; j < WIDTH; j++)
                if (save[i][j] < 0 && save[i][j - 1] == 0){
                    save[i][j - 1] = save[i][j];
                    save[i][j] = 0;
                }
                blockIdx[0] --;
            }
            
        } else if (key == 77){
            for (int i = 0; i < HEIGHT; i++)
            if (save[i][WIDTH - 1] < 0){
                flag = 0;
                break;
            }
            if (flag){
                for (int i = 0; i < HEIGHT; i++)
                for (int j = WIDTH - 2; j >= 0; j--)
                if (save[i][j] < 0 && save[i][j + 1] == 0){
                    save[i][j + 1] = save[i][j];
                    save[i][j] = 0;
                }
                blockIdx[0] ++;
            }
        }
        else if (key == 32) Rotate();
        else if (key == 80) fastFall = 1;
        else if (key == 99) SaveBlock();
    }
}


void Draw(){
    clock_t a = clock();
    float gameTime = (float)a / CLOCKS_PER_SEC;
    int gameMin = (int)gameTime / 60;
    int gameSec = (int)gameTime % 60;
	printNums(score,300,105); // 점수 
	printNums(gameMin,283,135); //분 
	printText(15,290,135); //: 출럭 
	printNums(gameSec,300,135); //초 

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            if (lastSave[i][j] != save[i][j]){
                COORD pos;
                pos.X = lt_pos[0] + j;
                pos.Y = lt_pos[1] + i;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
                // 12, 9, 10, 14
                switch (save[i][j]){
                    case 1:
                    case -1:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        break;
                    case -2:
                    case 2:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                        break;
                    case -3:
                    case 3:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                        break;
                    case -4:
                    case 4:
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                        break;
                }
                if(save[i][j] == 0) 
            		printf(" ");
            	else
            		printf("O");
                //printf("%s", save[i][j] == 0 ? " " : "■");
                //printf("%d", save[i][j]);
                lastSave[i][j] = save[i][j];
            }
        }
    }
}



void Setup(){
    nextBlockInfo[0] = rand() % 4 + 1;
    nextBlockInfo[1] = rand() % 7 + 1;
	printBoard();
    clock_t a = clock();
    float gameTime = (float)a / CLOCKS_PER_SEC;
    int gameMin = (int)gameTime / 60;
    int gameSec = (int)gameTime % 60;
	printNums(0,300,105); // 점수 
	printNums(gameMin,283,135); //분 
	printText(15,290,135); //: 출럭 
	printNums(gameSec,300,135); //초 
    srand(time(NULL));
    // system("mode con cols=100 lines=160 | title test");
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            save[i][j] = 0;
            lastSave[i][j] = 0;
            COORD pos;
            pos.X = j + lt_pos[0];
            pos.Y = i + lt_pos[1];
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
            if(save[i][j] == 0) 
            	printf(" ");
            else
            	printf("O");
            //printf("%s", save[i][j] == 0?" ":"ㅁ");
            //printf("%d", save[i][j]);
        }
        COORD pos;
        pos.X = lt_pos[0] + WIDTH;
        pos.Y = lt_pos[1] + i;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
        printf("\n");
    }
}

// void SummonBlock(){ // 0: 빈 공간 1: 조작할 수 없는 픽셀 // -1조작 가능한 픽셀  
//     save[0][WIDTH/2] = -1;
// }

void MoveSand(){
	/* 
	# Why MoveSand Works Asymmetrically
	the second for loop sweeps the line from left to right
	particles on the left that has no particle under them moves downward
	if a particle that has to move leftward, only that single particle moves leftward
	which results in jamming
	whereas paritcles moving rightward have no jamming at all
	
	moving left can be illustrated as this:
	(O is for sand particles to move, X is blank, M is rigid sand)
	(the code is currently sweeping at the top line)
	0		1		2		3		j index
	XOOO	XXOO	XXXO	XXXO
	XXXO -> XOXO -> XOOO -> XOOO
	XMMM	XMMM	XMMM	XMMM
	(particle visited in the 3th index is stuck, cannot move leftward) 
	
	where moving right can be illustrated as this: 
	0		1		2		3		j index
	XOOO	XXOO	XXXO	XXXO
	XOXX -> XOOX -> XOOO -> XOOO
	MMMX	MMMX	MMMX	MMMX
	
	thus, sweeping in one direction results in jamming for either moving leftward or rightward
	
	# How I Fixed This
	Fundamentally, premature move of downward results in jamming.
	I moved the code for moving downward to remove underlying cause.
	Then I could insert another for loop for moving particles leftward.
	This loop can let particles that should move leftward move simultaneously, getting rid of jamming.
	However, the two move function functions in a different sense, and can seem awkward.
	Whatever
	*/
    for (int i = HEIGHT - 2; i >= 0; i--){
        for (int j = 0; j < WIDTH; j++){
            if (save[i][j] != 0){
                if (save[i + 1][j] != 0){
                    if(j > 0 && save[i + 1][j - 1] == 0){
                    	int v = j;
                    	for(; v > 0 && save[i][v] != 0 && save[i + 1][v - 1] == 0 && save[i + 1][v - 1] != 0; --v);
						for(; v <= j; ++v){
                    		save[i + 1][v - 1] = save[i][v];
                    		save[i][v] = 0;
						}
                    }else if(j < WIDTH - 1 && save[i + 1][j + 1] == 0){
                        save[i + 1][j + 1] = save[i][j];
                        save[i][j] = 0;
                    }
                }
            }
        }
        for (int j = 0; j < WIDTH; j++){
            if (save[i][j] != 0){
                if (save[i + 1][j] == 0){
                    save[i + 1][j] = save[i][j];
                    save[i][j] = 0;
                }
            }
        }
    }blockIdx[1] += 1;
}

void DestroySand(){
	int visited[HEIGHT][WIDTH]={0,};
	for(int l=0;l<HEIGHT;++l){
		if(visited[l][0]||save[l][0]<1) continue;
		int queue[HEIGHT*WIDTH]={0,},qsize=1,yesclear=0;
		queue[0]=l*WIDTH;
		visited[l][0]=l+1;
		// look for same color mong-tang-ee
		for(int i=0;i<qsize;++i){
			yesclear=((queue[i]+1)%WIDTH<1)|yesclear;
			for(int j=0;j<4;++j){
				int cx=queue[i]/WIDTH+"1210"[j]-'1', cy=queue[i]%WIDTH+"0121"[j]-'1';
				if(cx<0||cy<0||cx>=HEIGHT||cy>=WIDTH||visited[cx][cy]||save[l][0]!=save[cx][cy]) continue;
				visited[cx][cy]=l+1;
				queue[qsize++]=cx*WIDTH+cy;
			}
		}
		// check if the line is erasable
		if(!yesclear) continue;
		// destroy sand
		qsize=1;
		queue[0]=l*WIDTH;
		visited[l][0]=0;
		for(int i=0;i<qsize;++i){
			// insert Score UP function here
            score += 100;
			save[queue[i]/WIDTH][queue[i]%WIDTH]=0;
			for(int j=0;j<4;++j){
				int cx=queue[i]/WIDTH+"1210"[j]-'1', cy=queue[i]%WIDTH+"0121"[j]-'1';
				if(cx<0||cy<0||cx>=HEIGHT||cy>=WIDTH||visited[cx][cy]!=l+1) continue;
				visited[cx][cy]=0;
				queue[qsize++]=cx*WIDTH+cy;
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
            queue[0]=i*WIDTH+j;
            for(int k=0;k<qsize;++k){
                yescollision=yescollision|(queue[k]/WIDTH==HEIGHT-1);
                for(int v=0;v<4;++v){
                    int cx=queue[k]/WIDTH+"1210"[v]-'1', cy=queue[k]%WIDTH+"0121"[v]-'1';
                    if(cx<0||cy<0||cx>=HEIGHT||cy>=WIDTH) continue;
                    if(cx-1==queue[k]/WIDTH&&save[cx][cy]>0) yescollision=1;
                    if(save[cx][cy]!=save[i][j]||visited[cx][cy]) continue;
                    visited[cx][cy]=1;
                    queue[qsize++]=cx*WIDTH+cy;
                }
            }if(!yescollision) return;
            qsize=1;
            queue[0]=i*WIDTH+j;
            save[i][j]=-save[i][j];
            for(int k=0;k<qsize;++k){
                for(int v=0;v<4;++v){
                    int cx=queue[k]/WIDTH+"1210"[v]-'1', cy=queue[k]%WIDTH+"0121"[v]-'1';
                    if(cx<0||cy<0||cx>=HEIGHT||cy>=WIDTH||save[cx][cy]!=-save[i][j]) continue;
                    save[cx][cy]=-save[cx][cy];
                    queue[qsize++]=cx*WIDTH+cy;
                }
            }SummonBlock();
            UpdateNext();
            canSave = 1;
            return;
        }
    }
}



void Summon(char block[][4][4], int idx){
    int x = WIDTH / 2 - 18;
    int y = 0;
    rotateNum = 0;
    // int color = rand() % 4 + 1;
    // currentBlockColor = color;
    blockIdx[0] = x;
    blockIdx[1] = y;
    for (int i = 0; i < 9 * 4; i++){
        for (int j = 0; j < 9 * 4; j++){
            if (block[idx][i / 9][j / 9] == 1 && save[y + i][x + j] != 0){
                system("cls");
                ended = 1;
                COORD pos = {0, 0};
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
                printf("  ________                        ________                     \n");
                printf(" /  _____/_____    _____   ____   \\_____  \\___  __ ___________ \n");
                printf("/   \\  ___\\__  \\  /     \\_/ __ \\   /   |   \\  \\/ // __ \\_  __ \\\n");
                printf("\\    \\_\\  \\/ __ \\|  Y Y  \\  ___/  /    |    \\   /\\  ___/|  | \\/\n");
                printf(" \\______  (____  /__|_|  /\\___  > \\_______  /\\_/  \\___  >__|   \n");
                printf("        \\/     \\/      \\/     \\/          \\/          \\/       ");
            }
            else if (block[idx][i / 9][j / 9] == 1) save[y + i][x + j] = block[idx][i / 9][j / 9] * currentBlockColor * -1;
        }
    }
}

// void Rotate()

void SummonBlock(){
    currentBlockColor = nextBlockInfo[0];
    // currentBlockColor = rand() % 4 + 1;
    switch (nextBlockInfo[1] - 1){
        case 0:
            currentBlock = IBlock;
            totRotateNum = 2;
            Summon(IBlock, 0);
            break;
        case 1:
            currentBlock = OBlock;
            totRotateNum = 1;
            Summon(OBlock, 0);
            break;
        case 2:
            currentBlock = TBlock;
            totRotateNum = 4;
            Summon(TBlock, 0);
            break;
        case 3:
            currentBlock = SBlock;
            totRotateNum = 2;
            Summon(SBlock, 0);
            break;
        case 4:
            currentBlock = ZBlock;
            totRotateNum = 2;
            Summon(ZBlock, 0);
            break;
        case 5:
            currentBlock = LBlock;
            totRotateNum = 4;
            Summon(LBlock, 0);
            break;
        default:
            currentBlock = JBlock;
            totRotateNum = 4;
            Summon(JBlock, 0);
            break;
    }
    nextBlockInfo[0] = rand() % 4 + 1;
    nextBlockInfo[1] = rand() % 7 + 1;
}

void Rotate(){
    int flag = 1;
    rotateNum++;
    if (rotateNum >= totRotateNum) rotateNum = 0;
    int x = blockIdx[0], y = blockIdx[1];
    int tmp = 1;
    while (tmp){
        tmp = 0;
        int k = 0;
        for (int i = 0; i < 9 * 4; i++)
        for (int j = 0; j < 9 * 4; j++){
            if (k) break;
            if (j + x >= WIDTH && currentBlock[rotateNum][i / 9][j / 9] == 1){
                // blockIdx[0] --;
                x --;
                tmp = 1;
                k = 1;
            }
            if (j + x < 0 && currentBlock[rotateNum][i / 9][j / 9] == 1){
                // blockIdx[0] ++;
                x ++;
                tmp = 1;
                k = 1;
            }
            if (i + y >= HEIGHT && currentBlock[rotateNum][i / 9][j / 9] == 1){
                y --;
                tmp = 1;
                k = 1;
            }
            if (i + y < 0 && currentBlock[rotateNum][i / 9][j / 9] == 1){
                y ++;
                tmp = 1;
                k = 1;
            }
        }
    }
    for (int i = 0; i < 9 * 4; i++)
    for (int j = 0; j < 9 * 4; j++)
    if (save[i + y][j + x] > 0 && currentBlock[rotateNum][i / 9][j / 9] == 1){
        flag = 0;
        break;
    }
    if (flag){
        for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
        if (save[i][j] < 0) save[i][j] = 0;
        for (int i = 0; i < 9 * 4; i++)
        for (int j = 0; j < 9 * 4; j++){
            if (currentBlock[rotateNum][i / 9][j / 9] == 1) save[i + y][j + x] = -currentBlockColor;
        }
        blockIdx[0] = x;
        blockIdx[1] = y;
    }
}


void SaveBlock(){
    if (canSave){
        canSave = 0;
        for (int i = 0; i < 9 * 4; i++)
        for (int j = 0; j < 9 * 4; j++)
        if (save[blockIdx[1] + i][blockIdx[0] + j] < 0) save[blockIdx[1] + i][blockIdx[0] + j] = 0;
        if (isSaved){
            SwitchBlock();
        }
        else{
            savedColor = currentBlockColor;
            savedRotateNum = totRotateNum;
            savedBlock = currentBlock;
            isSaved = 1;
            SummonBlock();
        }
        UpdateNext();
        gotoxy(200, 36);
        if (isSaved){
            switch (savedColor){
                case 1:
                case -1:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    break;
                case -2:
                case 2:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
                    break;
                case -3:
                case 3:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    break;
                case -4:
                case 4:
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    break;
            }

            for (int i = 0; i < 9 * 4; i++){
                for (int j = 0; j < 9 * 4; j++){
                    printf("%c", savedBlock[0][i / 9][j / 9] > 0 ? 'O' : ' ');
                }
                gotoxy(35, 36 + i + 1);
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        }
    }
}

void SwitchBlock(){
    char (*tmp)[4][4] = savedBlock;
    savedBlock = currentBlock;
    currentBlock = tmp;
    int tmptmp = currentBlockColor;
    currentBlockColor = savedColor;
    savedColor = tmptmp;
    int x = WIDTH / 2 - 18;
    int y = 0;
    rotateNum = 0;
    int ttmp = savedRotateNum;
    savedRotateNum = totRotateNum;
    totRotateNum = ttmp;
    blockIdx[0] = x;
    blockIdx[1] = y;
    for (int i = 0; i < 9 * 4; i++){
        for (int j = 0; j < 9 * 4; j++){
            if (currentBlock[rotateNum][i / 9][j / 9] == 1 && save[y + i][x + j] != 0){
                system("cls");
                ended = 1;
                COORD pos = {0, 0};
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
                printf("  ________                        ________                     \n");
                printf(" /  _____/_____    _____   ____   \\_____  \\___  __ ___________ \n");
                printf("/   \\  ___\\__  \\  /     \\_/ __ \\   /   |   \\  \\/ // __ \\_  __ \\\n");
                printf("\\    \\_\\  \\/ __ \\|  Y Y  \\  ___/  /    |    \\   /\\  ___/|  | \\/\n");
                printf(" \\______  (____  /__|_|  /\\___  > \\_______  /\\_/  \\___  >__|   \n");
                printf("        \\/     \\/      \\/     \\/          \\/          \\/       ");
            }
            else if (currentBlock[rotateNum][i / 9][j / 9] == 1) save[y + i][x + j] = currentBlock[rotateNum][i / 9][j / 9] * currentBlockColor * -1;
        }
    }
}

void UpdateNext(){
    gotoxy(223, 36);
    switch (nextBlockInfo[0]){
        case 1:
        case -1:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            break;
        case -2:
        case 2:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
            break;
        case -3:
        case 3:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            break;
        case -4:
        case 4:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            break;
    }
    char (*tmptmptmp)[4][4];
    switch (nextBlockInfo[1]){
        //iotszlj
        case 1:
            tmptmptmp = IBlock;
            break;
        case 2:
            tmptmptmp = OBlock;
            break;
        case 3:
            tmptmptmp = TBlock;
            break;
        case 4:
            tmptmptmp = SBlock;
            break;
        case 5:
            tmptmptmp = ZBlock;
            break;
        case 6:
            tmptmptmp = LBlock;
            break;
        case 7:
            tmptmptmp = JBlock;
            break;
    }
    for (int i = 0; i < 9 * 4; i++){
        for (int j = 0; j < 9 * 4; j++){
            printf("%c", tmptmptmp[0][i / 9][j / 9]>0?'O':' ');
        }
        gotoxy(235, 36 + i + 1);
    }
}
