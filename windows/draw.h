#ifndef _DRAW
#define _DRAW

#define SIZE 4
#define DEFAULT_COLOR 240
uint32_t score = 0;

// ����value��ȡ��Ӧ����ɫ������
void getColor(uint16_t value, HANDLE consolehwnd) {
	uint16_t color;
	color = DEFAULT_COLOR;//���ÿ���̨��ɫ��Ĭ��ֵ
	if (value > 0) while (value >>= 1)
	// value��������һλ��ֱ��ֵ��Ϊ0��ʵ��ÿ��������һ����ͬ����ɫ
	{
		color -= 15;
	}
	//Windows���ն˼�������ɫ����
	SetConsoleTextAttribute(consolehwnd, color);
}

// �������ݰ�,���ݰ干3��4�У�7��4��
void drawBoard(uint16_t board[SIZE][SIZE], HANDLE consolehwnd) {
	int8_t x, y;
	SetConsoleTextAttribute(consolehwnd, DEFAULT_COLOR);

	printf("|----------------------------|\n");
	printf("| by Zhengmingpei.github.com |\n");
	printf("|----------------------------|\n");
	printf("|      Your score:%-10d |\n", score);
	printf("|----------------------------|\n");
	//���ݰ干3��4�У�7��4��
	for (y = 0; y<SIZE; y++) {
		//���д�ӡ�հ�
		printf("|");
		for (x = 0; x<SIZE; x++) {
			getColor(board[x][y], consolehwnd);
			printf("       ");
			SetConsoleTextAttribute(consolehwnd, DEFAULT_COLOR);
		}
		printf("|\n|");
		//���д�ӡ����,���־���
		for (x = 0; x<SIZE; x++) {
			getColor(board[x][y], consolehwnd);
			if (board[x][y] != 0) {
				char s[8];
				//�˴�ע�⣬��board[x][y]������yx
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
		//ĩ�д�ӡ�հ�
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

// ����һά������x�����ϲ��������꣬stopΪ����
int8_t findTarget(uint16_t array[SIZE], int8_t x, int8_t stop) {
	int8_t t;
	//��xΪ��һ���������������ֱ�ӷ���x 
	if (x == 0) {
		return x;
	}
	//����x��ߵ�����
	for (t = x - 1; t >= 0; t--) {
		//�ϲ��㷨��
		//1.t��������Ϊ0����x����������ȣ�����t+1
		//2.t��������Ϊ0����x��������ȣ�����t
		//3.t������Ϊ0������stop�ж��Ƿ���ǰ���ң���ֹ��κϲ�
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

//��һά��������ƶ�
bool slideArray(uint16_t array[SIZE]) {
	bool success = false;
	//������ǰλ�ã����ϲ���λ�ã�����
	int8_t x, t, stop = 0;

	for (x = 0; x<SIZE; x++) {
		if (array[x] != 0) {
			t = findTarget(array, x, stop);
			// ������ϲ���λ���뵱ǰλ�ò���ȣ������ƶ����ߺϲ�
			// if target is not original position, then move or merge
			if (t != x) {
				// ������ϲ���λ�ò���0,���Ƽ���stop
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

//��ת���ݰ壬������ת90�ȣ�����������һ������������ƶ���ӿ����ĸ�������ƶ�
void rotateBoard(uint16_t board[SIZE][SIZE]) {
	int8_t i, j, n = SIZE;
	uint16_t tmp;
	//������ת��������ڣ��������
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

//�����ƶ����ݰ�
bool moveUp(uint16_t board[SIZE][SIZE]) {
	bool success = false;
	int8_t x;
	for (x = 0; x<SIZE; x++) {
		//��ÿһ�����ƶ����ߺϲ�����
		//�������ж������У���ǰ������˳���й�
		success |= slideArray(board[x]);
		//ֻҪ��һ�гɹ����ͳɹ�
	}
	return success;
}

// ���ƣ�������ת90�ȣ����Ϻϲ�������ת3��90��
bool moveLeft(uint16_t board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

// ���ƣ�������ת2��90�ȣ����Ϻϲ�������ת2��90��
bool moveDown(uint16_t board[SIZE][SIZE]) {
	bool success;
	rotateBoard(board);
	rotateBoard(board);
	success = moveUp(board);
	rotateBoard(board);
	rotateBoard(board);
	return success;
}

// ���ƣ�������ת3��90�ȣ����Ϻϲ�������ת1��90��
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

// �������ݰ��Ƿ�����
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

// �����Ϸ�Ƿ����
bool gameEnded(uint16_t board[SIZE][SIZE]) {
	bool ended = true;
	// ����п�λ��δ����
	if (countEmpty(board)>0) return false;
	// �����飬�������������δ����
	if (findPairDown(board)) return false;
	rotateBoard(board);
	// ��תһ�Σ������飬�������������δ����
	if (findPairDown(board)) ended = false;
	rotateBoard(board);
	rotateBoard(board);
	rotateBoard(board);
	return ended;
}

// ����������ݰ�
void addRandom(uint16_t board[SIZE][SIZE]) {
	// ȫ�ֱ������Ƿ��ѳ�ʼ��
	static bool initialized = false;
	// x,y ����
	int8_t x, y;
	// r ���λ�ã�len ����Ϊ�յ����ݰ����ݳ���
	int16_t r, len = 0;
	// n ������ݣ� list ����Ϊ�յ����ݰ�λ��
	int8_t n, list[SIZE*SIZE][2];

	if (!initialized) {
		srand(time(NULL));
		initialized = true;
	}

	// �ҳ����ݰ�������Ϊ�յ�����
	for (x = 0; x<SIZE; x++) {
		for (y = 0; y<SIZE; y++) {
			if (board[x][y] == 0) {
				list[len][0] = x;
				list[len][1] = y;
				len++;
			}
		}
	}

	// �����Ϊ�յ���������������
	if (len>0) {
		r = rand() % len;
		x = list[r][0];
		y = list[r][1];
		n = ((rand() % 10) / 9 + 1) * 2;
		board[x][y] = n;
	}
}


#endif