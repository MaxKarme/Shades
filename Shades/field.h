#pragma once
#include "blockColor.h"
#include "framework.h"

void initField(int x, int y, int width, int height);
void drawField(HDC hdc);
void moveCurrent();
void changeColl(int vector, HWND hWnd);
void BlockAnimationsTick();
void increaseSpeed();
void changeFieldTheme(int newTheme);
struct BlockColor getColorFromThemes(int i, int j);
void saveField();
void restartField(int width, int height);

int getPoints();
int getRecordPosition();
