#include "field.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "animation.h"


struct ActiveBlock {
	int coll, y, type;
	int speed, baseSpeed;
	int currentLineTop;
};

struct Field {
	int x, y;
	int rows, colls, sizeX, sizeY, currentTheme;
	struct ActiveBlock current;
	int animationsCount;
	int points;
	int recordPosition;
};

struct BlockAnimation {
	RECT coords;
	int isVisible;
	int deletedLine;
	int checkEmptyColl;
	int animationId;
};


struct BlockAnimation fieldAnimations[10];

struct BlockColor themes[3][6] = {
	{{100, 100, 100}, {252, 254, 217}, {249, 241, 133}, {255, 146, 3}, {197, 66, 1}, {114, 11, 2}},
	{{150, 150, 150}, {255, 255, 255}, {140, 204, 239}, {29, 140, 212}, {32, 86, 194}, {9, 17, 38}},
	{{150, 150, 150}, {255, 255, 255}, {196, 174, 212}, {163, 84, 150}, {117, 15, 99}, {66, 0, 90}}
};

struct Field field;
int** map;

struct BlockColor getColorFromThemes(int i, int j) {
	return themes[i][j];
}

void checkGameOver(int coll);

void initField(int x, int y, int width, int height) {
	srand(time(NULL));


	field.x = x;
	field.y = y;

	FILE* file;
	fopen_s(&file, "test1.txt", "rt");
	if (!file) return;

	field.rows = 10;
	fscanf_s(file, "%d", &field.colls);

	map = (int**)malloc(field.rows * sizeof(int*));

	for (int i = 0; i < field.rows; ++i) {
		map[i] = (int*)malloc(field.colls * sizeof(int));
		for (int j = 0; j < field.colls; ++j) fscanf_s(file, "%d", &map[i][j]);
	}

	fscanf_s(file, "%d", &field.points);
	fscanf_s(file, "%d", &field.recordPosition);

	fscanf_s(file, "%d", &field.current.y);
	fscanf_s(file, "%d", &field.current.coll);
	fscanf_s(file, "%d", &field.current.type);

	field.animationsCount = 0;

	field.sizeX = width / field.colls;
	field.sizeY = height / field.rows;
	field.currentTheme = getTheme();

	field.current.baseSpeed = height / 50;
	field.current.speed = height / 50;

	field.current.currentLineTop = getCollTop(0);

	for (int i = 0; i < field.colls; ++i) {
		checkGameOver(i);
	}

	fclose(file);
}

void restartField(int width, int height) {
	field.animationsCount = 0;
	field.points = 0;
	field.recordPosition = -1;

	field.rows = 10;
	field.colls = getCollsCount();
	field.currentTheme = getTheme();

	field.sizeX = width / field.colls;
	field.sizeY = height / field.rows;

	field.current.coll = 0;
	field.current.y = 0;
	field.current.type = rand() % 4 + 1;
	field.current.baseSpeed = height / 50;
	field.current.speed = height / 50;

	map = (int**)realloc(map, field.rows * sizeof(int*));

	for (int i = 0; i < field.rows; ++i) {
		map[i] = (int*)realloc(map[i], field.colls * sizeof(int));
		for (int j = 0; j < field.colls; ++j) map[i][j] = 0;
	}

	field.current.currentLineTop = getCollTop(0);
}



int getPoints() { return field.points; };
int getRecordPosition() { return field.recordPosition; };



void increaseSpeed() {
	if (field.current.speed > field.current.baseSpeed) return;
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



void mergeColl(int coll, int row) {
	row++;
	if (row >= field.rows) return;
	int isFirstAnimation = 1;

	while (map[row][coll] == map[row - 1][coll]) {
		if (map[row][coll] == 5) return;

		field.points++;

		struct BlockColor currentColor = themes[field.currentTheme][map[row][coll]];

		RECT coords = { coll * field.sizeX + field.x, (row - 1) * field.sizeY + field.y, 0, 0 };

		coords.right = coords.left + field.sizeX;
		coords.bottom = coords.top + field.sizeY;

		struct BlockAnimation disappearBlock = { coords, isFirstAnimation, -1, -1 }; // coords, isVisible, deletedLine, checkEmptyInColl

		struct Animation disappearAnimation;
		initAnimation(&disappearAnimation);

		disappearAnimation.color = currentColor;
		disappearAnimation.coords = coords;
		disappearAnimation.id = getId();
		disappearAnimation.steps = 10;
		disappearAnimation.isActive = isFirstAnimation;
		disappearAnimation.delta.color = getDeltaColor(currentColor, themes[field.currentTheme][0], 10);

		disappearBlock.animationId = disappearAnimation.id;

		fieldAnimations[field.animationsCount++] = disappearBlock;

		if (field.animationsCount) {
			performAfter(disappearAnimation, fieldAnimations[field.animationsCount - 2].animationId);
		}
		else performNow(disappearAnimation);

		struct BlockColor endColor = themes[field.currentTheme][map[row][coll] + 1];
		coords.top += field.sizeY;
		coords.bottom += field.sizeY;

		struct BlockAnimation increaseBlock = { coords, 1, -1, -1 }; // coords, isVisible, deletedLine, checkEmptyInColl

		struct Animation increaseAnimation;
		initAnimation(&increaseAnimation);

		increaseAnimation.color = currentColor;
		increaseAnimation.coords = coords;
		increaseAnimation.id = getId();
		increaseAnimation.steps = 10;
		increaseAnimation.isActive = isFirstAnimation;
		increaseAnimation.delta.color = getDeltaColor(currentColor, endColor, 10);

		increaseBlock.animationId = increaseAnimation.id;

		fieldAnimations[field.animationsCount++] = increaseBlock;

		performParallelWith(increaseAnimation, fieldAnimations[field.animationsCount - 2].animationId);


		map[row - 1][coll] = 0;
		map[row][coll] += 1;


		row++;
		isFirstAnimation = 0;

		if (row >= field.rows) break;
	};

	field.current.currentLineTop = getCollTop(field.current.coll);

	if (isFirstAnimation) return; // нет слияний

	fieldAnimations[field.animationsCount - 1].checkEmptyColl = coll;
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

	field.points += 5 * map[deletedLine][0];

	struct BlockColor currentColor = themes[field.currentTheme][map[deletedLine][0]];

	RECT coords = { field.x, deletedLine * field.sizeY + field.y, 0, 0 };

	coords.right = coords.left + field.sizeX * field.colls;
	coords.bottom = coords.top + field.sizeY;

	struct BlockAnimation disappearLine = { coords, !field.animationsCount, deletedLine, -1 }; // coords, isVisible, deletedLine, checkEmptyInColl

	struct Animation disappearAnimation;
	initAnimation(&disappearAnimation);

	disappearAnimation.color = currentColor;
	disappearAnimation.coords = coords;
	disappearAnimation.id = getId();
	disappearAnimation.steps = 10;
	disappearAnimation.isActive = !field.animationsCount;
	disappearAnimation.delta.color = getDeltaColor(currentColor, themes[field.currentTheme][0], 10);

	disappearLine.animationId = disappearAnimation.id;

	fieldAnimations[field.animationsCount++] = disappearLine;

	if (field.animationsCount > 1) {
		performAfter(disappearAnimation, fieldAnimations[field.animationsCount - 2].animationId);
	}
	else performNow(disappearAnimation);

	
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

void checkGameOver(int coll) {
	if (map[0][coll] == 0) return;
	if (field.recordPosition != -1) {
		changeState(4);
		return;
	}

	FILE* file;
	fopen_s(&file, "records.txt", "rt");

	if (file == NULL) return;

	int length;
	fscanf_s(file, "%d", &length);

	length >>= 4; // расшифровка
	length -= 3; // расшифровка

	int* arr = (int*)malloc(sizeof(int) * (length + 1));

	int current;

	for (int i = 0; i < length; ++i) {
		fscanf_s(file, "%d", &current);
		current >>= 4; // расшифровка
		current -= 3; // расшифровка

		if (field.points > current && field.recordPosition == -1) {
			arr[i] = field.points;
			field.recordPosition = ++i;
		}

		arr[i] = current;
	}

	if (field.recordPosition == -1) {
		arr[length] = field.points;
		field.recordPosition = length + 1;
	}

	fclose(file);

	file = fopen("records.txt", "wt");
	if (file == NULL) return;

	if (length > 9) length = 9;
	length++;

	fprintf_s(file, "%d ", ((length + 3) << 4)); // шифровка

	for (int i = 0; i < length; ++i) {
		fprintf_s(file, "%d ", ((arr[i] + 3) << 4)); // шифровка
	}

	fclose(file);

	changeState(4);
}

void moveCurrent() {
	int topCoords = field.rows * field.sizeY - field.current.currentLineTop * field.sizeY;
	if (field.animationsCount) return;
	if (field.current.y + field.current.speed >= topCoords - field.sizeY) {
		addCurrentInMap();
		mergeColl(field.current.coll, field.rows - field.current.currentLineTop - 1);
		checkLines();
		checkGameOver(field.current.coll);

		field.current.coll = 0;
		field.current.y = 0;

		field.current.currentLineTop = getCollTop(0);
		field.current.type = rand() % 4 + 1;
		field.current.speed = field.current.baseSpeed;

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

void doneCheck(int index) {
	deleteAnimation(fieldAnimations[index].animationId);
	if (fieldAnimations[index].deletedLine != -1) deleteLine(fieldAnimations[index].deletedLine);
	if (fieldAnimations[index].checkEmptyColl != -1) checkEmptyInColl(fieldAnimations[index].checkEmptyColl);

	for (int i = index; i < field.animationsCount - 1; ++i) {
		fieldAnimations[i] = fieldAnimations[i + 1];
	}

	field.animationsCount--;
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
					field.current.coll * field.sizeX + field.sizeX + field.x, field.current.y + field.sizeY + field.y
				};

	if(!field.animationsCount) FillRect(hdc, &coords, hBrush);
	DeleteObject(hBrush);

	for (int i = 0; i < field.animationsCount; ++i) {

		if (!fieldAnimations[i].isVisible && !isActive(fieldAnimations[i].animationId)) continue;


		hBrush = CreateSolidBrush(getColorByAnimationId(fieldAnimations[i].animationId));
		SelectObject(hdc, hBrush);

		RECT BlockCoords = getCoordsByAnimationId(fieldAnimations[i].animationId);

		FillRect(hdc, &BlockCoords, hBrush);

		if (isDone(fieldAnimations[i].animationId)) {
			doneCheck(i);
			i--;
			continue;
		}

		DeleteObject(hBrush);
	}
}

void changeFieldTheme(int newTheme) {
	field.currentTheme = newTheme;
}

void saveField() {
	FILE* file;
	fopen_s(&file, "map.txt", "wt");

	if (!file) return;

	fprintf(file, "%d\n", field.colls);

	for (int i = 0; i < field.rows; ++i) {
		for (int j = 0; j < field.colls; ++j) {
			fprintf_s(file, "%d ", map[i][j]);
		}
		fprintf(file, "\n");
	}

	fprintf(file, "%d ", field.points);
	fprintf(file, "%d\n", field.recordPosition);

	fprintf(file, "%d ", field.current.y);
	fprintf(file, "%d ", field.current.coll);
	fprintf(file, "%d", field.current.type);


	fclose(file);
}
