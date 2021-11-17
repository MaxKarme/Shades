#pragma once
#include "framework.h"
#include "Panel.h"

void initMenu(int x, int y, int width, int height);
void drawMenu(hdc);
int getMenuState();

int MenuOnMouseMove(int x, int y);
int MenuOnMouseDown(int x, int y);
int MenuOnMouseUp();

struct Menu {
	int width, height, x, y;
	struct Panel* panels;
	int length;
	int state;
};

void changeState(int newState);