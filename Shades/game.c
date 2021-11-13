#include "game.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>


struct ActiveBlock {
	int coll, y, type, speed, currentLineTop;
};

struct Game {
	int rows, colls, sizeX, sizeY, currentTheme;
	struct ActiveBlock current;
	int animationsCount;
};

struct BlockColor {
	double r, g, b;
};

struct ColorAnimation {
	struct BlockColor current, delta;
	int steps;
	int isActive;
};

struct BlockAnimation {
	int x, y;
	int id, nextId;
	int isVisible;
	struct ColorAnimation a;
};

struct BlockAnimation animations[10];

struct BlockColor themes[2][6] = {
	{{100, 100, 100}, {252, 254, 217}, {249, 241, 133}, {255, 146, 3}, {197, 66, 1}, {114, 11, 2}},
	{{150, 150, 150}, {255, 255, 255}, {255, 0, 0}, {255, 255, 0}, {0, 255, 0}, {0, 0, 255}}
};

struct Game game;
int** map;

void init() {
	srand(time(NULL));

	game.animationsCount = 0;

	game.rows = 10;
	game.colls = 4;
	game.sizeX = 70;
	game.sizeY = 40;
	game.currentTheme = 0;

	game.current.coll = 0;
	game.current.y = 0;
	game.current.type = rand() % 4 + 1;
	game.current.speed = 8;
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

int getId() {
	static int id = 1;
	return id++;
}

void BlockAnimationsTick() {
	for (int i = 0; i < game.animationsCount; ++i) {
		if (!animations[i].a.isActive) continue;
		animations[i].a.current.r += animations[i].a.delta.r;
		animations[i].a.current.g += animations[i].a.delta.g;
		animations[i].a.current.b += animations[i].a.delta.b;

		animations[i].a.steps--;

		if (animations[i].a.steps) continue;

		int nextId = animations[i].nextId;

		for (int j = i + 1; j < game.animationsCount; ++j) {
			if (animations[j].id == nextId) {
				animations[j].a.isActive = 1;
				animations[j].isVisible = 1;
				nextId = 0;
			}
			animations[j - 1] = animations[j];
		}

		i--;
		game.animationsCount--;

		if (!nextId) continue;

		for (int j = 0; nextId && j < game.animationsCount; ++j) {
			if (animations[j].id == nextId) {
				animations[j].a.isActive = 1;
				nextId = 0;
			}
		}
	}
}

struct BlockColor getDelta(struct BlockColor from, struct BlockColor to, int steps) {
	struct BlockColor res = { (to.r - from.r) / steps, (to.g - from.g) / steps, (to.b - from.b) / steps };
	return res;
}

void mergeColl(int index) {
	int row = 0;
	while (map[row][index] == 0) row++;

	row++;
	if (row >= game.rows) return;

	int isFirstAnimation = 1;

	while (map[row][index] == map[row - 1][index]) {
		if (map[row][index] == 5) return;

		int id = getId();
		struct BlockColor currentColor = themes[game.currentTheme][map[row][index]];

		struct BlockAnimation disappearA = {index * game.sizeX, (row - 1) * game.sizeY, id, 0, isFirstAnimation}; // x, y, id, nextId, isVisible
		struct ColorAnimation disappearColor = { currentColor, getDelta(currentColor, themes[game.currentTheme][0], 6), 6, isFirstAnimation}; // currentColor deltaColor steps isActive

		disappearA.a = disappearColor;

		if (!isFirstAnimation) {
			animations[game.animationsCount - 2].nextId = id;
		}

		game.animationsCount++;
		animations[game.animationsCount - 1] = disappearA;

		id = getId();
		struct BlockColor endColor = themes[game.currentTheme][map[row][index] + 1];

		struct BlockAnimation increaseA = { index * game.sizeX, row * game.sizeY, id, 0, 1 }; // x, y, id, nextId, isVisible
		struct ColorAnimation increaseColor = { currentColor, getDelta(currentColor, endColor, 6), 6, isFirstAnimation }; // currentColor deltaColor steps isActive

		increaseA.a = increaseColor;

		isFirstAnimation = 0;

		if (!isFirstAnimation) {
			animations[game.animationsCount - 2].nextId = id;
		}

		game.animationsCount++;
		animations[game.animationsCount - 1] = increaseA;

		map[row - 1][index] = 0;
		map[row][index] += 1;

		row++;
		if (row >= game.rows) return;
	};
}

void checkLines() {
	int check = 1;
	int deletedLine = 0;

	for (int i = 0; i < game.rows; ++i) {
		int currentType = map[i][0];
		if (currentType == 0) continue;

		check = 1;

		for (int j = 1; j < game.colls; ++j) {
			if (map[i][j] != currentType) {
				check = 0;
				break;
			}
		}

		if (check) {
			deletedLine = i;
			break;
		}
	}

	if (!check) return;

	for (int i = deletedLine; i > 0; --i) {
		for (int j = 0; j < game.colls; ++j) {
			map[i][j] = map[i - 1][j];
		}
	}

	for (int i = 0; i < game.colls; ++i) map[0][i] = 0;
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
	if (game.current.y + game.current.speed >= topCoords - game.sizeY) {
		addCurrentInMap();
		mergeColl(game.current.coll);
		checkLines();

		game.current.coll = 0;
		game.current.y = 0;

		game.current.currentLineTop = getCollTop(0);
		game.current.type = rand() % 4 + 1;

		return;
	}
	game.current.y += game.current.speed;
}

void drawItem(HDC hdc, int row, int coll, int type) {
	struct BlockColor currentC = themes[game.currentTheme][type];

	HBRUSH hBrush = CreateSolidBrush(RGB(currentC.r, currentC.g, currentC.b));
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


	struct BlockColor currentC = themes[game.currentTheme][game.current.type];
	HBRUSH hBrush = CreateSolidBrush(RGB(currentC.r, currentC.g, currentC.b));

	RECT coords = { game.current.coll * game.sizeX, game.current.y, game.current.coll * game.sizeX + game.sizeX, game.current.y + game.sizeY };

	FillRect(hdc, &coords, hBrush);
	DeleteObject(hBrush);

	for (int i = 0; i < game.animationsCount; ++i) {
		if (!animations[i].isVisible) continue;

		hBrush = CreateSolidBrush(RGB(animations[i].a.current.r, animations[i].a.current.g, animations[i].a.current.b));
		SelectObject(hdc, hBrush);

		RECT BlockCoords = { animations[i].x, animations[i].y, animations[i].x + game.sizeX, animations[i].y + game.sizeY };
		FillRect(hdc, &BlockCoords, hBrush);

		DeleteObject(hBrush);
	}
}