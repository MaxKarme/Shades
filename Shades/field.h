#pragma once
#include "framework.h"

void initField(int x, int y, int width, int height);
void drawField(HDC hdc);
void moveCurrent();
void changeColl(int vector, HWND hWnd);
void BlockAnimationsTick();
void increaseSpeed();
int getPoints();

struct Field;