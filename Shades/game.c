#include "game.h"
//#define _CRT_SECURE_NO_WARNINGS;
#include <stdio.h>


COLORREF themes[1][6] = {
	{RGB(120, 120, 120), RGB(255, 255, 255), RGB(250, 220, 120), RGB(250, 120, 0), RGB(200, 60, 0), RGB(110, 12, 2)}
};

struct ActiveBlock {
	int coll, y, type, speed, currentLineTop;
};

struct Game {
	int rows, colls, sizeX, sizeY, currentTheme;
	struct ActiveBlock current;
};

struct Game game;
int** map;

void init() {
	game.rows = 10;
	game.colls = 4;
	game.sizeX = 50;
	game.sizeY = 20;
	game.currentTheme = 0;

	game.current.coll = 0;
	game.current.y = 0;
	game.current.type = 2;
	game.current.speed = 4;
	game.current.currentLineTop = 0;

	map = (int**)malloc(game.rows * sizeof(int*));

	for (int i = 0; i < game.rows; ++i) {
		map[i] = (int*)malloc(game.colls * sizeof(int));
		for (int j = 0; j < game.colls; ++j) map[i][j] = 0;
	}
}

int printMap() {
	FILE* file = fopen("log.txt", "wt");

	for (int i = 0; i < game.rows; ++i) {
		for (int j = 0; j < game.colls; ++j) fprintf(file, "%d ", map[i][j]);
		fprintf(file, "\n");
	}

	fclose(file);
}

int getCollTop(int index) {
	for (int i = 0; i < game.rows; ++i) {
		if (map[i][index] != 0) {
			return game.rows - i;
		}
	}

	return 0;
}

void changeColl(int vector, HWND hWnd) {
	int newColl = game.current.coll + vector;
	if (newColl < 0 || newColl >= game.colls) return;
	game.current.coll = newColl;
	game.current.currentLineTop = getCollTop(game.current.coll);
}

void addCurrentInMap() {
	for (int i = 1; i < game.rows; ++i) {
		if (map[i][game.current.coll] != 0) {
			map[i - 1][game.current.coll] = game.current.type;
			return;
		}
	}

	map[game.rows - 1][game.current.coll] = game.current.type;
}

void moveCurrent() {
	int topCoords = game.rows * game.sizeY - game.current.currentLineTop * game.sizeY;
	if (game.current.y >= topCoords - game.sizeY) {
		addCurrentInMap();
		game.current.coll = 0;
		game.current.y = 0;
		game.current.currentLineTop = getCollTop(0);
		return;
	}
	game.current.y += game.current.speed;
}

void drawItem(HDC hdc, int row, int coll, int type) {
	HBRUSH hBrush = CreateSolidBrush(themes[game.currentTheme][type]);
	RECT coords = { coll * game.sizeX, row * game.sizeY, coll * game.sizeX + game.sizeX, row * game.sizeY + game.sizeY };

	FillRect(hdc, &coords, hBrush);
	DeleteObject(hBrush);
}


void drawField(HDC hdc) {
	for (int i = 0; i < game.rows; ++i) {
		for (int j = 0; j < game.colls; ++j) {
			drawItem(hdc, i, j, map[i][j]);
		}
	}

	HBRUSH hBrush = CreateSolidBrush(themes[game.currentTheme][game.current.type]);
	RECT coords = { game.current.coll * game.sizeX, game.current.y, game.current.coll * game.sizeX + game.sizeX, game.current.y + game.sizeY };

	FillRect(hdc, &coords, hBrush);
	DeleteObject(hBrush);
}