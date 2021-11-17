#include "field.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>


struct ActiveBlock {
	int coll, y, type, speed, currentLineTop;
};

struct Field {
	int x, y;
	int rows, colls, sizeX, sizeY, currentTheme;
	struct ActiveBlock current;
	int animationsCount;
	int points;
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
	int width, height;
	int x, y;
	int id, nextId;
	int isVisible;
	int deletedLine;
	int checkEmptyColl;
	struct ColorAnimation a;
};


struct BlockAnimation animations[10];

struct BlockColor themes[2][6] = {
	{{100, 100, 100}, {252, 254, 217}, {249, 241, 133}, {255, 146, 3}, {197, 66, 1}, {114, 11, 2}},
	{{150, 150, 150}, {255, 255, 255}, {255, 0, 0}, {255, 255, 0}, {0, 255, 0}, {0, 0, 255}}
};

struct Field field;
int** map;

void initField(int x, int y, int width, int height) {
	srand(time(NULL));

	field.animationsCount = 0;
	field.points = 0;

	field.x = x;
	field.y = y;

	field.rows = 10;
	field.colls = 4;
	field.sizeX = width / field.colls;
	field.sizeY = height / field.rows;
	field.currentTheme = 0;

	field.current.coll = 0;
	field.current.y = 0;
	field.current.type = rand() % 4 + 1;
	field.current.speed = 8;
	field.current.currentLineTop = 0;

	map = (int**)malloc(field.rows * sizeof(int*));

	for (int i = 0; i < field.rows; ++i) {
		map[i] = (int*)malloc(field.colls * sizeof(int));
		for (int j = 0; j < field.colls; ++j) map[i][j] = 0;
	}
}

int getPoints() { return field.points; };

int printMap() {
	FILE* file = fopen("log.txt", "wt");

	for (int i = 0; i < field.rows; ++i) {
		for (int j = 0; j < field.colls; ++j) fprintf(file, "%d ", map[i][j]);
		fprintf(file, "\n");
	}

	fclose(file);
}

int getId() {
	static int id = 1;
	return id++;
}

void increaseSpeed() {
	if (field.current.speed > 8) return;
	field.current.speed *= 2;
}

void mergeColl(int coll, int row);
void checkLines();

int deleteLine(int deletedLine) {
	for (int i = deletedLine; i > 0; --i) {
		for (int j = 0; j < field.colls; ++j) {
			map[i][j] = map[i - 1][j];
		}
	}

	for (int i = 0; i < field.colls; ++i) map[0][i] = 0;

	field.current.currentLineTop--;

	for (int i = 0; i < field.colls; ++i) mergeColl(i, deletedLine);
	checkLines();
}

void deleteEmpty(int coll, int row, int height) {
	for (int i = row; i - height >= 0; --i) {
		map[i][coll] = map[i - height][coll];
	}

	for (int i = 0; i < height; ++i) {
		map[i][coll] = 0;
	}

	mergeColl(coll, row);
	checkLines();
}

void checkEmptyInColl(int coll) {
	int wasElem = 0;
	int emptyIndex = -1;
	int emptyHeight = 0;

	for (int i = 0; i < field.rows; ++i) {
		if (map[i][coll] != 0) wasElem = 1;
		if (!wasElem || map[i][coll] != 0) continue;

		emptyIndex = i;
		emptyHeight++;
	}
	if (emptyIndex == -1) return;

	deleteEmpty(coll, emptyIndex, emptyHeight);
}

void BlockAnimationsTick() {
	for (int i = 0; i < field.animationsCount; ++i) {
		if (!animations[i].a.isActive) continue;
		animations[i].a.current.r += animations[i].a.delta.r;
		animations[i].a.current.g += animations[i].a.delta.g;
		animations[i].a.current.b += animations[i].a.delta.b;

		animations[i].a.steps--;

		if (animations[i].a.steps) continue;

		int nextId = animations[i].nextId;
		if (animations[i].deletedLine != -1) deleteLine(animations[i].deletedLine);
		if (animations[i].checkEmptyColl != -1) checkEmptyInColl(animations[i].checkEmptyColl);

		for (int j = i + 1; j < field.animationsCount; ++j) {
			if (animations[j].id == nextId) {
				animations[j].a.isActive = 1;
				animations[j].isVisible = 1;
				nextId = 0;
			}
			animations[j - 1] = animations[j];
		}

		i--;
		field.animationsCount--;

		if (!nextId) continue;

		for (int j = 0; nextId && j < field.animationsCount; ++j) {
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

void mergeColl(int coll, int row) {
	row++;
	if (row >= field.rows) return;
	int isFirstAnimation = 1;

	while (map[row][coll] == map[row - 1][coll]) {
		if (map[row][coll] == 5) return;

		int id = getId();
		struct BlockColor currentColor = themes[field.currentTheme][map[row][coll]];

		struct BlockAnimation disappearA = {
												field.sizeX, field.sizeY,						// width, heigth
												coll * field.sizeX, (row - 1) * field.sizeY,	// x, y
												id, 0, isFirstAnimation, -1, -1				// id nextId, isVisible, deletedLine, checkEmptyColl
											};

		struct ColorAnimation disappearColor = { currentColor, getDelta(currentColor, themes[field.currentTheme][0], 6), 6, isFirstAnimation}; // currentColor deltaColor steps isActive

		disappearA.a = disappearColor;

		if (!isFirstAnimation) {
			animations[field.animationsCount - 2].nextId = id;
		}

		field.animationsCount++;
		animations[field.animationsCount - 1] = disappearA;

		id = getId();
		struct BlockColor endColor = themes[field.currentTheme][map[row][coll] + 1];

		struct BlockAnimation increaseA = {
											field.sizeX, field.sizeY,					// width, heigth
											coll * field.sizeX, row * field.sizeY,	// x, y
											id, 0, 1, -1, -1						// id nextId, isVisible, deletedLine, checkEmptyColl
										};

		struct ColorAnimation increaseColor = { currentColor, getDelta(currentColor, endColor, 6), 6, isFirstAnimation }; // currentColor deltaColor steps isActive

		increaseA.a = increaseColor;


		if (!isFirstAnimation) {
			animations[field.animationsCount - 2].nextId = id;
		}

		field.animationsCount++;
		animations[field.animationsCount - 1] = increaseA;

		map[row - 1][coll] = 0;
		map[row][coll] += 1;

		row++;
		isFirstAnimation = 0;

		if (row >= field.rows) break;
	};

	if (isFirstAnimation) return; // нет слияний

	animations[field.animationsCount - 1].checkEmptyColl = coll;
}

void checkLines() {
	int check = 1;
	int deletedLine = 0;

	for (int i = 0; i < field.rows; ++i) {
		int currentType = map[i][0];
		if (currentType == 0) {
			check = 0;
			continue;
		}

		check = 1;

		for (int j = 1; j < field.colls; ++j) {
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

	int id = getId();
	field.points += 5 * map[deletedLine][0];

	struct BlockAnimation disappearLine = {
										field.sizeX * field.colls, field.sizeY,			// width, heigth
										0, deletedLine * field.sizeY,					// x, y
										id, 0, !field.animationsCount, deletedLine, -1	// id nextId, isVisible, deletedLine, checkEmptyColl
	};

	struct BlockColor currentColor = themes[field.currentTheme][map[deletedLine][0]];
	struct ColorAnimation disappearColor = { currentColor, getDelta(currentColor, themes[field.currentTheme][0], 6), 6, !field.animationsCount }; // currentColor deltaColor steps isActive

	disappearLine.a = disappearColor;
	if(field.animationsCount) animations[field.animationsCount - 1].nextId = id;

	field.animationsCount++;
	animations[field.animationsCount - 1] = disappearLine;
}

int getCollTop(int index) {
	for (int i = 0; i < field.rows; ++i) {
		if (map[i][index] != 0) {
			return field.rows - i;
		}
	}

	return 0;
}

void changeColl(int vector, HWND hWnd) {
	int newColl = field.current.coll + vector;
	if (newColl < 0 || newColl >= field.colls) return;

	int topCoords = field.rows * field.sizeY - getCollTop(newColl) * field.sizeY;
	if (field.current.y >= topCoords - field.sizeY) return;
	
	field.current.coll = newColl;
	field.current.currentLineTop = getCollTop(field.current.coll);
}

void addCurrentInMap() {
	for (int i = 1; i < field.rows; ++i) {
		if (map[i][field.current.coll] != 0) {
			map[i - 1][field.current.coll] = field.current.type;
			return;
		}
	}

	map[field.rows - 1][field.current.coll] = field.current.type;
}

void moveCurrent() {
	int topCoords = field.rows * field.sizeY - field.current.currentLineTop * field.sizeY;
	//if (field.animationsCount) return;
	if (field.current.y + field.current.speed >= topCoords - field.sizeY) {
		addCurrentInMap();
		mergeColl(field.current.coll, field.rows - field.current.currentLineTop - 1);
		checkLines();

		field.current.coll = 0;
		field.current.y = 0;

		field.current.currentLineTop = getCollTop(0);
		field.current.type = rand() % 4 + 1;
		field.current.speed = 8;

		return;
	}
	field.current.y += field.current.speed;
}


void drawItem(HDC hdc, int row, int coll, int type) {
	struct BlockColor currentC = themes[field.currentTheme][type];

	HBRUSH hBrush = CreateSolidBrush(RGB(currentC.r, currentC.g, currentC.b));
	RECT coords = { 
					coll * field.sizeX + field.x, row * field.sizeY + field.y,
					coll * field.sizeX + field.sizeX + field.x, row * field.sizeY + field.sizeY + field.y
				};

	FillRect(hdc, &coords, hBrush);
	DeleteObject(hBrush);
}

void drawField(HDC hdc) {
	for (int i = 0; i < field.rows; ++i) {
		for (int j = 0; j < field.colls; ++j) {
			drawItem(hdc, i, j, map[i][j]);
		}
	}


	struct BlockColor currentC = themes[field.currentTheme][field.current.type];
	HBRUSH hBrush = CreateSolidBrush(RGB(currentC.r, currentC.g, currentC.b));

	RECT coords = { 
					field.current.coll * field.sizeX + field.x,	field.current.y + field.y, 
					field.current.coll * field.sizeX + field.sizeX, field.current.y + field.sizeY + field.y
				};

	FillRect(hdc, &coords, hBrush);
	DeleteObject(hBrush);

	for (int i = 0; i < field.animationsCount; ++i) {
		if (!animations[i].isVisible) continue;

		hBrush = CreateSolidBrush(RGB(animations[i].a.current.r, animations[i].a.current.g, animations[i].a.current.b));
		SelectObject(hdc, hBrush);

		RECT BlockCoords = {
							animations[i].x + field.x, animations[i].y + field.y, 
							animations[i].x + animations[i].width + field.x, animations[i].y + animations[i].height + field.y
						};

		FillRect(hdc, &BlockCoords, hBrush);

		DeleteObject(hBrush);
	}
}