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

void pauseBtnClick() {
	changeState(1);
}

void continueBtnClick() {
	changeState(0);
}

void restartBtnClick() {
	initField(0, 50, 280, 400);
}

void settingsBtnClick() {
	changeState(2);
}