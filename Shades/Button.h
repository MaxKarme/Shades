#pragma once
#include "framework.h"
#include "Menu.h"

struct Button {
	int width, height, x, y;
	LPCTSTR text;
	int isVisible;
	int hovered, mouseDown;
	void (*onclick) ();
};

void drawButton(HDC hdc, struct Button btn);
void pauseBtnClick();
void continueBtnClick();
void restartBtnClick();
void settingsBtnClick();