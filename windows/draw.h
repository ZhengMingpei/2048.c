#ifndef _DRAW
#define _DRAW

#define SIZE 4
#define DEFAULT_COLOR 240
uint32_t score = 0;

// 根据value获取相应的颜色并设置
void getColor(uint16_t value, HANDLE consolehwnd) {
	uint16_t color;
	color = DEFAULT_COLOR;//设置控制台颜色的默认值
	if (value > 0) while (value >>= 1)
	// value不断右移一位，直到值变为0，实现每个二进制一个不同的颜色
	{
		color -= 15;
	}
	//Windows下终端及字体颜色设置
	SetConsoleTextAttribute(consolehwnd, color);
}

// 绘制数据板,数据板共3×4行，7×4列
void drawBoard(uint16_t board[SIZE][SIZE], HANDLE consolehwnd) {
	int8_t x, y;
	SetConsoleTextAttribute(consolehwnd, DEFAULT_COLOR);

	printf("|----------------------------|\n");
	printf("| by Zhengmingpei.github.com |\n");
	printf("|----------------------------|\n");
	printf("|      Your score:%-10d |\n", score);
	printf("|----------------------------|\n");
	//数据板共3×4行，7×4列
	for (y = 0; y<SIZE; y++) {
		//首行打印空白
		printf("|");
		for (x = 0; x<SIZE; x++) {
			getColor(board[x][y], consolehwnd);
			printf("       ");
			SetConsoleTextAttribute(consolehwnd, DEFAULT_COLOR);
		}
		printf("|\n|");
		//次行打印数字,数字居中
		for (x = 0; x<SIZE; x++) {
			getColor(board[x][y], consolehwnd);
			if (board[x][y] != 0) {
				char s[8];
				//此处注意，是board[x][y]而不是yx
				sprintf_s(s, 8, "%u", board[x][y]);
				int8_t t = 7 - strlen(s);
				printf("%*s%s%*s", t - t / 2, "", s, t / 2, "");
			}
			else {
				printf("   .   ");
			}
			SetConsoleTextAttribute(consolehwnd, DEFAULT_COLOR);
		}
		printf("|\n|");
		//末行打印空白
		for (x = 0; x<SIZE; x++) {
			getColor(board[x][y], consolehwnd);
			printf("       ");
			SetConsoleTextAttribute(consolehwnd, DEFAULT_COLOR);
		}
		printf("|\n");
	}
	printf("|----------------------------|\n");
	printf("|        w,a,s,d or q        |\n");
}

// 查找一维数组中x左侧待合并数的坐标，stop为检查点
int8_t findTarget(uint16_t array[SIZE], int8_t x, int8_t stop) {
	int8_t t;
	//若x为第一个数，左边无数，直接返回x 
	if (x == 0) {
		return x;
	}
	//遍历x左边的坐标
	for (t = x - 1; t >= 0; t--) {
		//合并算法：
		//1.t处的数不为0且与x处的数不相等，返回t+1
		//2.t处的数不为0且与x处的数相等，返回t
		//3.t处的数为0，根据stop判断是否向前查找，防止多次合并
		if (array[t] != 0) {
			if (array[t] != array[x]) {
				// merge is not possible, take next position
				return t + 1;
			}
			return t;
		}
		else {
			// we should not slide further, return this one
			if (t == stop) {
				return t;
			}
		}
	}
	// we did not find a
	return x;
}

//对一维数组进行移动
bool slideArray(uint16_t array[SIZE]) {
	bool success = false;
	//声明当前位置，待合并的位置，检查点
	int8_t x, t, stop = 0;

	for (x = 0; x<SIZE; x++) {
		if (array[x] != 0) {
			t = findTarget(array, x, stop);
			// 如果待合并的位置与当前位置不相等，进行移动或者合并
			// if target is not original position, then move or merge
			if (t != x) {
				// 如果待合并的位置不是0,右移检查点stop
				// if target is not zero, set stop to avoid double merge
				if (array[t] != 0) {
					score += array[t] + array[x];
					stop = t + 1;
				}
				array[t] += array[x];
				array[x] = 0;
				success = true;
			}
		}
	}
	return success;
}

//旋转数据板，向右旋转90度，这样可以用一个方向的数组移动间接控制四个方向的移动
void rotateBoard(uint16_t board[SIZE][SIZE]) {
	int8_t i, j, n = SIZE;
	uint16_t tmp;
	//环形旋转，先外而内，先左后右
	for (i = 0; i<n / 2; i++){
		for (j = i; j<n - i - 1; j++){
			tmp = board[i][j];
			board[i][j] = board[j][n - i - 1];
			board[j][n - i - 1] = board[n - i - 1][n - j - 1];
			board[n - i - 1][n - j - 1] = board[n - j - 1][i];
			board[n - j - 1][i] = tmp;
		}
	}
}

//向上移动数据板
bool moveUp(uint16_t board[SIZE][SIZE]) {
	bool success = false;
	int8_t x;
	for (x = 0; x<SIZE; x++) {
		//对每一列做移动或者合并处理，
		//这里是列而不是行，与前面的输出顺序有关
		success |= slideArray(board[x]);
		//只要有一列成功，就成功
	}
	return success;
}

// 左移：向右旋转90度，向上合并，再旋转3个90度
bool moveLeft(uint16_t board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

// 下移：向右旋转2个90度，向上合并，再旋转2个90度
bool moveDown(uint16_t board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

// 右移：向右旋转3个90度，向上合并，再旋转1个90度
bool moveRight(uint16_t board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	return success;
}

bool findPairDown(uint16_t board[SIZE][SIZE]) {
	bool success = false;
	int8_t x, y;
	for (x = 0; x<SIZE; x++) {
		for (y = 0; y<SIZE - 1; y++) {
			if (board[x][y] == board[x][y + 1]) return true;
		}
	}
	return success;
}

// 计算数据板是否已满
int16_t countEmpty(uint16_t board[SIZE][SIZE]) {
	int8_t x, y;
	int16_t count = 0;
	for (x = 0; x<SIZE; x++) {
		for (y = 0; y<SIZE; y++) {
			if (board[x][y] == 0) {
				count++;
			}
		}
	}
	return count;
}

// 检查游戏是否结束
bool gameEnded(uint16_t board[SIZE][SIZE]) {
	bool ended = true;
	// 如果有空位，未结束
	if (countEmpty(board)>0) return false;
	// 横向检查，有相等相邻数，未结束
	if (findPairDown(board)) return false;
	rotateBoard(board);
	// 旋转一次，纵向检查，有相等相邻数，未结束
	if (findPairDown(board)) ended = false;
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return ended;
}

// 随机重置数据板
void addRandom(uint16_t board[SIZE][SIZE]) {
	// 全局变量，是否已初始化
	static bool initialized = false;
	// x,y 坐标
	int8_t x, y;
	// r 随机位置，len 所有为空的数据板数据长度
	int16_t r, len = 0;
	// n 随机数据， list 所有为空的数据板位置
	int8_t n, list[SIZE*SIZE][2];

	if (!initialized) {
		srand(time(NULL));
		initialized = true;
	}

	// 找出数据板上所有为空的坐标
	for (x = 0; x<SIZE; x++) {
		for (y = 0; y<SIZE; y++) {
			if (board[x][y] == 0) {
				list[len][0] = x;
				list[len][1] = y;
				len++;
			}
		}
	}

	// 如果有为空的情况，才填充数据
	if (len>0) {
		r = rand() % len;
		x = list[r][0];
		y = list[r][1];
		n = ((rand() % 10) / 9 + 1) * 2;
		board[x][y] = n;
	}
}


#endif