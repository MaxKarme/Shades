#include "game.h"

COLORREF themes[1][6] = {
	{RGB(120, 120, 120), RGB(255, 255, 255), RGB(250, 220, 120), RGB(250, 120, 0), RGB(200, 60, 0), RGB(110, 12, 2)}
};

struct Game {
	int rows, colls, sizeX, sizeY, currentTheme;
};

struct Game game;
int** map;

void init() {
	game.rows = 10;
	game.colls = 4;
	game.sizeX = 50;
	game.sizeY = 20;
	game.currentTheme = 0;

	map = (int**)malloc(game.rows * sizeof(int*));

	for (int i = 0; i < game.rows; ++i) {
		map[i] = (int*)malloc(game.colls * sizeof(int));
		for (int j = 0; j < game.colls; ++j) map[i][j] = 0;
	}

	map[9][1] = 1;
	map[5][2] = 2;
	map[1][1] = 3;
	map[9][2] = 4;
	map[8][3] = 5;
}

void drawItem(HDC hdc, int row, int coll, int type) {
	HBRUSH hBrush = CreateSolidBrush(themes[game.currentTheme][type]);
	RECT coords = { coll * game.sizeX, row * game.sizeY, coll * game.sizeX + game.sizeX, row * game.sizeY + game.sizeY };

	FillRect(hdc, &coords, hBrush);
}


void drawField(HDC hdc) {
	for (int i = 0; i < game.rows; ++i) {
		for (int j = 0; j < game.colls; ++j) {
			drawItem(hdc, i, j, map[i][j]);
		}
	}
}