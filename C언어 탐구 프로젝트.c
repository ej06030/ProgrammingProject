#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define FRAME_X 90
#define FRAME_Y 150

BOARD[FRAME_Y][FRAME_X] = {0};	//1: 빨간 픽셀 2: 파란 픽샐 3: 초록 픽셀 4: 노란 픽셀 0: 공백 -1 ~ -4: 플레이어가 움직일 수 있는 픽셀
visted[FRAME_X];

void blockFall(); //블록(9 * 9 픽셀) 이 한 칸 떨어짐 
void pixelFall(); //픽셀이 한 픽셀 떨어짐 
void checkCollide(); //픽셀 충돌 연산 (수직, 좌/우 45도 각도)
void checkClear(); //
