#include "Button.h";
#include <string.h>;
#include <winuser.h>;


void drawButton(HDC hdc, struct Button btn) {
	RECT rect = {btn.x, btn.y, btn.x + btn.width, btn.y + btn.height};

	HBRUSH defaultBrush = CreateSolidBrush(RGB(220, 220, 220));
	HBRUSH hoveredBrush = CreateSolidBrush(RGB(200, 200, 200));
	HBRUSH mouseDownBrush = CreateSolidBrush(RGB(170, 170, 170));

	if (btn.mouseDown) {
		FillRect(hdc, &rect, mouseDownBrush);
		SetBkColor(hdc, RGB(170, 170, 170));
	}
	else if (btn.hovered) {
		FillRect(hdc, &rect, hoveredBrush);
		SetBkColor(hdc, RGB(200, 200, 200));
	}
	else {
		FillRect(hdc, &rect, defaultBrush);
		SetBkColor(hdc, RGB(220, 220, 220));
	}
	
	DrawText(hdc, btn.text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	DeleteObject(defaultBrush);
	DeleteObject(hoveredBrush);
	DeleteObject(mouseDownBrush);
}

void drawBorder(HDC hdc, struct Button btn, COLORREF color) {
	int lineW = 4;

	HPEN hPen = CreatePen(PS_SOLID, lineW, color);
	SelectObject(hdc, hPen);

	MoveToEx(hdc, btn.x - lineW / 2, btn.y - lineW / 2, NULL);
	LineTo(hdc, btn.x + btn.width + lineW / 2, btn.y - lineW / 2);
	LineTo(hdc, btn.x + btn.width + lineW / 2, btn.y + btn.height + lineW / 2);
	LineTo(hdc, btn.x - lineW / 2, btn.y + btn.height + lineW / 2);
	LineTo(hdc, btn.x - lineW / 2, btn.y - lineW / 2);
}

void drawDisableButton(HDC hdc, struct Button btn) {
	RECT rect = { btn.x, btn.y, btn.x + btn.width, btn.y + btn.height };

	HBRUSH disableBrush = CreateSolidBrush(RGB(140, 140, 140));

	FillRect(hdc, &rect, disableBrush);
	SetBkColor(hdc, RGB(140, 140, 140));

	DrawText(hdc, btn.text, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	DeleteObject(disableBrush);
}


void pauseBtnClick(int btnValue) {
	if (getMenuState() == 4) return;
	changeState(1);
}

void continueBtnClick(int btnValue) {
	changeState(0);
}

void restartBtnClick(int btnValue) {
	restartGame();
	if (getMenuState() == 4) changeState(1);
}

void settingsBtnClick(int btnValue) {
	changeState(2);
}

void changeCollsCountBtnClick(int btnValue) {
	changeCollsCountSetting(btnValue);
}

void changeThemeBtnClick(int btnValue) {
	changeThemeSetting(btnValue);
}

void appLyButtonClick() {
	if (!wasSettingsChanges()) return;

	if (applyChanges()) changeState(3);
}

void settingsMessageBtnClick() {
	changeState(2);
}

void backButtonClick() {
	changeState(1);
}

void fameHallClick() {
	changeState(5);
}