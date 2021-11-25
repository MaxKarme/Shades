#pragma once
#include "framework.h"
#include "Menu.h"
#include "settings.h"
#include "field.h"

struct Button {
	int width, height, x, y;
	LPCTSTR text;
	int isVisible;
	int hovered, mouseDown;
	void (*onclick) (int btnValue);
	int value; // опционально
};

void drawBorder(HDC hdc, struct Button btn, COLORREF color);
void drawButton(HDC hdc, struct Button btn);
void drawDisableButton(HDC hdc, struct Button btn);

void pauseBtnClick();
void continueBtnClick();
void restartBtnClick();
void settingsBtnClick();
void changeCollsCountBtnClick(int btnValue);
void changeThemeBtnClick(int btnValue);
void appLyButtonClick();
void backButtonClick();
void settingsMessageBtnClick();
void fameHallClick();