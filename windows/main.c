#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include <Windows.h>
#include <conio.h>
#include "draw.h"

int main()
{
	uint16_t board[SIZE][SIZE];
	char c;
	bool success;
	//创建句柄，详细句柄知识，请百度一下或查MSDN
	HANDLE consolehwnd;
	//实例化句柄
	consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(consolehwnd, DEFAULT_COLOR);
	system("title 2048.c");
	system("mode con cols=32 lines=23");
	system("cls");
	memset(board, 0, sizeof(board));
	addRandom(board);
	addRandom(board);
	system("cls");
	drawBoard(board,consolehwnd);

	while (true)
	{
		c = _getch();
		switch (c)
		{
			case 97: //a
			case 104://h
			case 65: //A
			case 72: //H
				success = moveLeft(board);
				break;
			case 100://d
			case 108://l
			case 68: //D
			case 76: //L
				success = moveRight(board);
				break;
			case 119://w
			case 107://k
			case 87: //W
			case 75: //K
				success = moveUp(board);
				break;
			case 115://s
			case 106://j
			case 83: //S
			case 74: //J
				success = moveDown(board);
				break;

			default:
				success = false;
				break;
		}
			
		if (success)
		{
			//drawBoard(board, consolehwnd);
			Sleep(50);

			addRandom(board);
			system("cls");
			drawBoard(board, consolehwnd);
			if (gameEnded(board))
			{
				printf("        GAME OVER!         \n");
				Sleep(2000);
				break;
			}
		}

		if (c == 'q')
		{
			printf("         QUIT? (y/n):");
			while (true)
			{
				c = getchar();
				if (c == 'y')
				{
					system("cls");
					exit(0);
				}
				else if (c == 'n')
				{
					system("cls");
					drawBoard(board, consolehwnd);
					break;
				}
			}
		}
	}
	return 0;
}