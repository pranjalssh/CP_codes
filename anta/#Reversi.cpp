#include <cstdio>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;

char board[8][9];

void check_and_reverse(char player, int y, int x, int dy, int dx) {
	int yy = y + dy, xx = x + dx;
	while(1) {
		if(!(0 <= yy && yy < 8 && 0 <= xx && xx < 8)) return;
		char c = board[yy][xx];
		if(c == '.') return;
		if(c == player) break;
		yy += dy; xx += dx;
	}
	while(1) {
		yy -= dy; xx -= dx;
		if(yy == y && xx == x) break;
		board[yy][xx] = player;
	}
}

int main() {
	for(int y = 0; y < 8; y ++)
		for(int x = 0; x < 8; x ++)
			board[y][x] = '.';
	board[3][3] = 'W';
	board[3][4] = 'B';
	board[4][3] = 'B';
	board[4][4] = 'W';

	int N;
	scanf("%d", &N);
	for(int i = 0; i < N; i ++) {
		static char buf[2];
		int x, y;
		scanf("%s%d%d", buf, &x, &y);
		x --, y --;
		board[y][x] = buf[0];
		for(int dy = -1; dy <= 1; dy ++)
			for(int dx = -1; dx <= 1; dx ++)
				if(dy != 0 || dx != 0)
					check_and_reverse(buf[0], y, x, dy, dx);
	}
	int Bs = 0, Ws = 0;
	for(int y = 0; y < 8; y ++) {
		for(int x = 0; x < 8; x ++) {
			char c = board[y][x];
			if(c == 'B') Bs ++;
			if(c == 'W') Ws ++;
		}
	}
	printf("%02d-%02d %s\n", Bs, Ws,
		Bs == Ws ? "Draw!" : Bs > Ws ? "The black won!" : "The white won!");
	return 0;
}
