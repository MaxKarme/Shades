#include "Panel.h"
#include <stdio.h>

void drawTopPanel(HDC hdc, struct Menu menu, struct Panel panel) {
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 180, 0));
	RECT rect = { panel.x, panel.y, panel.x + panel.width, panel.y + panel.height };

	FillRect(hdc, &rect, hBrush);

	wchar_t points[10];
	int numLength = swprintf(&points, 10, L"%d", getPoints());

	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, points, numLength, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	for (int i = 0; i < panel.length; ++i) {
		drawButton(hdc, panel.btns[i]);
	}

	DeleteObject(hBrush);
};

void drawPausePanel(HDC hdc, struct Menu menu, struct Panel panel) {
	if (menu.state != 1) return;

	HBRUSH hBrush = CreateSolidBrush(RGB(0, 180, 0));
	RECT rect = { panel.x, panel.y, panel.x + panel.width, panel.y + panel.height };

	FillRect(hdc, &rect, hBrush);

	for (int i = 0; i < panel.length; ++i) {
		drawButton(hdc, panel.btns[i]);
	}

	DeleteObject(hBrush);
};

void drawSettingsPanel(HDC hdc, struct Menu menu, struct Panel panel) {
	if (menu.state != 2) return;

	HBRUSH hBrush = CreateSolidBrush(RGB(180, 0, 0));
	RECT rect = { panel.x, panel.y, panel.x + panel.width, panel.y + panel.height };

	FillRect(hdc, &rect, hBrush);

	for (int i = 0; i < panel.length; ++i) {
		drawButton(hdc, panel.btns[i]);
	}

	DeleteObject(hBrush);
};

void getTopPanel(struct Menu menu, int index) {
	menu.panels[index].width = menu.width;
	menu.panels[index].height = 50;
	menu.panels[index].x = menu.x;
	menu.panels[index].y = menu.y;

	menu.panels[index].length = 1;
	menu.panels[index].btns = (struct Button*)malloc(sizeof(struct Button) * 1);

	menu.panels[index].btns[0].x = (menu.x + menu.width - 80);
	menu.panels[index].btns[0].y = menu.y + 5;
	menu.panels[index].btns[0].width = 60;
	menu.panels[index].btns[0].height = 40;

	menu.panels[index].btns[0].text = L"pause";
	menu.panels[index].btns[0].isVisible = 1;
	menu.panels[index].btns[0].hovered = 0;
	menu.panels[index].btns[0].mouseDown = 0;

	menu.panels[index].btns[0].onclick = &pauseBtnClick;

	menu.panels[index].draw = &drawTopPanel;
}


void getPausePanel(struct Menu menu, int index) {
	menu.panels[index].width = 150;
	menu.panels[index].height = 300;
	menu.panels[index].x = (menu.width - 150) / 2 + menu.x;
	menu.panels[index].y = (menu.height - 300) / 2 + menu.y;

	menu.panels[index].length = 3;
	menu.panels[index].btns = (struct Button*)malloc(sizeof(struct Button) * 3);

	for (int i = 0; i < 3; ++i) {
		menu.panels[index].btns[i].x = menu.panels[index].x + 25;
		menu.panels[index].btns[i].y = menu.panels[index].y + 70 + 60 * i;
		menu.panels[index].btns[i].width = 100;
		menu.panels[index].btns[i].height = 40;

		menu.panels[index].btns[i].isVisible = 1;
		menu.panels[index].btns[i].hovered = 0;
		menu.panels[index].btns[i].mouseDown = 0;
	}

	menu.panels[index].btns[0].text = L"Continue";
	menu.panels[index].btns[0].onclick = &continueBtnClick;

	menu.panels[index].btns[1].text = L"Restart";
	menu.panels[index].btns[1].onclick = &restartBtnClick;

	menu.panels[index].btns[2].text = L"Settings";
	menu.panels[index].btns[2].onclick = &settingsBtnClick;

	menu.panels[index].draw = &drawPausePanel;
}

void getSettingsPanel(struct Menu menu, int index) {
	menu.panels[index].width = 180;
	menu.panels[index].height = 300;
	menu.panels[index].x = (menu.width - 180) / 2 + menu.x;
	menu.panels[index].y = (menu.height - 300) / 2 + menu.y;

	menu.panels[index].length = 6;
	menu.panels[index].btns = (struct Button*)malloc(sizeof(struct Button) * 6);

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 3; ++j) {
			int currentIndex = i * 3 + j;

			menu.panels[index].btns[currentIndex].x = menu.panels[index].x + 20 + j * 50;
			menu.panels[index].btns[currentIndex].y = menu.panels[index].y + 100 + 60 * i;
			menu.panels[index].btns[currentIndex].width = 40;
			menu.panels[index].btns[currentIndex].height = 40;

			menu.panels[index].btns[currentIndex].isVisible = 1;
			menu.panels[index].btns[currentIndex].hovered = 0;
			menu.panels[index].btns[currentIndex].mouseDown = 0;
		}
	}

	menu.panels[index].btns[0].text = L"th1";
	//menu.panels[index].btns[0].onclick = &continueBtnClick;

	menu.panels[index].btns[1].text = L"th2";
	//menu.panels[index].btns[1].onclick = &restartBtnClick;

	menu.panels[index].btns[2].text = L"th3";
	//menu.panels[index].btns[2].onclick = &settingsBtnClick;

	
	menu.panels[index].btns[3].text = L"th1";
	//menu.panels[index].btns[0].onclick = &continueBtnClick;

	menu.panels[index].btns[4].text = L"th2";
	//menu.panels[index].btns[1].onclick = &restartBtnClick;

	menu.panels[index].btns[5].text = L"th3";
	//menu.panels[index].btns[2].onclick = &settingsBtnClick;

	menu.panels[index].draw = &drawSettingsPanel;
}