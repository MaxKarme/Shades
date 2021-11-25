#include "Panel.h"
#include <stdio.h>

void drawTopPanel(HDC hdc, struct Menu* menu, struct Panel* panel) {
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 180, 0));
	RECT rect = { panel->x, panel->y, panel->x + panel->width, panel->y + panel->height };

	FillRect(hdc, &rect, hBrush);

	wchar_t points[10];
	int numLength = swprintf(&points, 10, L"%d", getPoints());

	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, points, numLength, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	for (int i = 0; i < panel->length; ++i) {
		drawButton(hdc, panel->btns[i]);
	}

	DeleteObject(hBrush);
};

void drawPausePanel(HDC hdc, struct Menu* menu, struct Panel* panel) {
	if (menu->state != 1) {
		panel->isActive = 0;
		return;
	}

	panel->isActive = 1;

	HBRUSH hBrush = CreateSolidBrush(RGB(0, 180, 0));
	RECT rect = { panel->x, panel->y, panel->x + panel->width, panel->y + panel->height };

	FillRect(hdc, &rect, hBrush);

	for (int i = 0; i < panel->length; ++i) {
		drawButton(hdc, panel->btns[i]);
	}

	DeleteObject(hBrush);
};

void drawSettingsMessagePanel(HDC hdc, struct Menu* menu, struct Panel* panel) {
	if (menu->state != 3) {
		panel->isActive = 0;
		return;
	}

	panel->isActive = 1;


	HBRUSH hBrush = CreateSolidBrush(RGB(230, 190, 100));
	RECT panelRect = { panel->x, panel->y, panel->x + panel->width, panel->y + panel->height };

	FillRect(hdc, &panelRect, hBrush);


	RECT textRect = { panel->x, panel->y + 40, panel->x + panel->width, panel->y + 70 };

	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, L"Some changes will\n apply in next game", -1, &textRect, DT_CENTER | DT_VCENTER);

	//textRect.top += 20;
	//textRect.bottom += 20;

	//DrawText(hdc, L"apply in next game", -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	drawButton(hdc, panel->btns[0]);
}


void drawThemeBtn(HDC hdc, struct Button btn, int theme) {
	struct BlockColor color = getColorFromThemes(theme, 1);
	HBRUSH hBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));

	RECT rect = { btn.x, btn.y, btn.x + btn.width / 2, btn.y + btn.height / 2 };
	FillRect(hdc, &rect, hBrush);

	DeleteObject(hBrush);


	color = getColorFromThemes(theme, 2);
	hBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));

	rect.left	= btn.x + btn.width / 2;	
	rect.top	= btn.y;
	rect.right	= btn.x + btn.width;		
	rect.bottom	= btn.y + btn.height / 2;

	FillRect(hdc, &rect, hBrush);

	DeleteObject(hBrush);


	color = getColorFromThemes(theme, 3);
	hBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));

	rect.left = btn.x;
	rect.top = btn.y + btn.height / 2;
	rect.right = btn.x + btn.width / 2;
	rect.bottom = btn.y + btn.height;

	FillRect(hdc, &rect, hBrush);


	DeleteObject(hBrush);

	color = getColorFromThemes(theme, 4);
	hBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));

	rect.left = btn.x + btn.width / 2;
	rect.top = btn.y + btn.height / 2;
	rect.right = btn.x + btn.width;
	rect.bottom = btn.y + btn.height;

	FillRect(hdc, &rect, hBrush);

	DeleteObject(hBrush);
}

void drawSettingsBorder(HDC hdc, struct Button btn) {
	if (getCollsCount() == btn.value || getTheme() == btn.value) drawBorder(hdc, btn, RGB(150, 80, 80));
	if (getChangedCollsCount() == btn.value || getChangedTheme() == btn.value) drawBorder(hdc, btn, RGB(255, 0, 0));

	if (btn.hovered) drawBorder(hdc, btn, RGB(255, 0, 0));
	if (btn.mouseDown) drawBorder(hdc, btn, RGB(180, 0, 0));
}

void drawSettingsPanel(HDC hdc, struct Menu* menu, struct Panel* panel) {
	if (menu->state != 2) {
		panel->isActive = 0;
		return;
	}

	panel->isActive = 1;


	HBRUSH hBrush = CreateSolidBrush(RGB(180, 180, 180));
	RECT rect = { panel->x, panel->y, panel->x + panel->width, panel->y + panel->height };

	FillRect(hdc, &rect, hBrush);

	for (int i = 0; i < panel->length - 2; ++i) {
		if(i < 3) drawButton(hdc, panel->btns[i]);
		else drawThemeBtn(hdc, panel->btns[i], i % 3);
		drawSettingsBorder(hdc, panel->btns[i]);
	}

	drawButton(hdc, panel->btns[6]);

	if (!wasSettingsChanges()) drawDisableButton(hdc, panel->btns[7]);
	else drawButton(hdc, panel->btns[7]);


	DeleteObject(hBrush);

	RECT textRect = { panel->x, panel->y + 40, panel->x + panel->width, panel->y + 70 };
	DrawText(hdc, L"Collumns Count:", -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	textRect.top += 100;
	textRect.bottom += 100;

	DrawText(hdc, L"Theme:", -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
};


void drawGameOverPanel(HDC hdc, struct Menu* menu, struct Panel* panel) {
	if (menu->state != 4) {
		panel->isActive = 0;
		return;
	}

	panel->isActive = 1;

	HBRUSH hBrush = CreateSolidBrush(RGB(240, 83, 83));
	RECT rect = { panel->x, panel->y, panel->x + panel->width, panel->y + panel->height };

	FillRect(hdc, &rect, hBrush);

	drawButton(hdc, panel->btns[0]);

	rect.top += 20;

	wchar_t points[50];
	int length = swprintf(&points, 50, L"Your points count: %d", getPoints());

	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, points, length, &rect, DT_SINGLELINE | DT_CENTER);

	rect.top += 30;

	wchar_t recordPosText[40];
	int recordPos = getRecordPosition();

	if (recordPos == -1) {
		length = swprintf(&recordPosText, 50, L"Your record position: -");
	}
	else {
		length = swprintf(&recordPosText, 50, L"Your record position: %3d", recordPos);
	}

	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, recordPosText, length, &rect, DT_SINGLELINE | DT_CENTER);
}


void drawRecords(HDC hdc, RECT rect) {
	FILE* file;
	fopen_s(&file, "records.txt", "rt");

	if (file == NULL) return;

	int length;
	fscanf_s(file, "%d", &length);

	length >>= 4; // расшифровка
	length -= 3; // расшифровка

	rect.top += 20;

	DrawText(hdc, L"Hall Of Fame", -1, &rect, DT_CENTER);

	rect.top += 50;
	rect.left += 20;

	int buffer[20];
	int current;

	for (int i = 0; i < length; ++i) {
		fscanf_s(file, "%d", &current);
		current >>= 4; // расшифровка
		current -= 3; // расшифровка 

		int length = swprintf(buffer, 20, L"%2d. %d", i + 1, current);

		DrawText(hdc, buffer, length, &rect, 0);

		rect.top += 20;
	}

	fclose(file);
}

void drawFameHallPanel(HDC hdc, struct Menu* menu, struct Panel* panel) {
	if (menu->state != 5) {
		panel->isActive = 0;
		return;
	}

	panel->isActive = 1;

	HBRUSH hBrush = CreateSolidBrush(RGB(239, 239, 80));
	RECT rect = { panel->x, panel->y, panel->x + panel->width, panel->y + panel->height };

	FillRect(hdc, &rect, hBrush);

	drawButton(hdc, panel->btns[0]);

	drawRecords(hdc, rect);
}



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

	menu.panels[index].isActive = 1;
}

void getPausePanel(struct Menu menu, int index) {
	menu.panels[index].width = 150;
	menu.panels[index].height = 300;
	menu.panels[index].x = (menu.width - 150) / 2 + menu.x;
	menu.panels[index].y = (menu.height - 300) / 2 + menu.y;

	menu.panels[index].length = 4;
	menu.panels[index].btns = (struct Button*)malloc(sizeof(struct Button) * 4);

	for (int i = 0; i < 4; ++i) {
		menu.panels[index].btns[i].x = menu.panels[index].x + 25;
		menu.panels[index].btns[i].y = menu.panels[index].y + 40 + 60 * i;
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

	menu.panels[index].btns[2].text = L"Hall of fame";
	menu.panels[index].btns[2].onclick = &fameHallClick;

	menu.panels[index].btns[3].text = L"Settings";
	menu.panels[index].btns[3].onclick = &settingsBtnClick;

	menu.panels[index].draw = &drawPausePanel;

	if(menu.state == 1) menu.panels[index].isActive = 1;
	else menu.panels[index].isActive = 0;
}

void getSettingsPanel(struct Menu menu, int index) {
	menu.panels[index].width = 180;
	menu.panels[index].height = 300;
	menu.panels[index].x = (menu.width - 180) / 2 + menu.x;
	menu.panels[index].y = (menu.height - 300) / 2 + menu.y;

	menu.panels[index].length = 8;
	menu.panels[index].btns = (struct Button*)malloc(sizeof(struct Button) * 8);

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 3; ++j) {
			int currentIndex = i * 3 + j;

			menu.panels[index].btns[currentIndex].x = menu.panels[index].x + 20 + j * 50;
			menu.panels[index].btns[currentIndex].y = menu.panels[index].y + 80 + 100 * i;
			menu.panels[index].btns[currentIndex].width = 40;
			menu.panels[index].btns[currentIndex].height = 40;

			menu.panels[index].btns[currentIndex].isVisible = 1;
			menu.panels[index].btns[currentIndex].hovered = 0;
			menu.panels[index].btns[currentIndex].mouseDown = 0;
		}
	}

	menu.panels[index].btns[0].text = L"3";
	menu.panels[index].btns[0].value = 3;
	menu.panels[index].btns[0].onclick = &changeCollsCountBtnClick;

	menu.panels[index].btns[1].text = L"4";
	menu.panels[index].btns[1].value = 4;
	menu.panels[index].btns[1].onclick = &changeCollsCountBtnClick;

	menu.panels[index].btns[2].text = L"5";
	menu.panels[index].btns[2].value = 5;
	menu.panels[index].btns[2].onclick = &changeCollsCountBtnClick;

	
	menu.panels[index].btns[3].text = L"";
	menu.panels[index].btns[3].value = 0;
	menu.panels[index].btns[3].onclick = &changeThemeBtnClick;

	menu.panels[index].btns[4].text = L"";
	menu.panels[index].btns[4].value = 1;
	menu.panels[index].btns[4].onclick = &changeThemeBtnClick;

	menu.panels[index].btns[5].text = L"";
	menu.panels[index].btns[5].value = 2;
	menu.panels[index].btns[5].onclick = &changeThemeBtnClick;


	menu.panels[index].btns[6].x = menu.panels[index].x + 30;
	menu.panels[index].btns[6].y = menu.panels[index].y + 240;
	menu.panels[index].btns[6].width = 50;
	menu.panels[index].btns[6].height = 30;

	menu.panels[index].btns[6].isVisible = 1;
	menu.panels[index].btns[6].hovered = 0;
	menu.panels[index].btns[6].mouseDown = 0;

	menu.panels[index].btns[6].text = L"Back";
	menu.panels[index].btns[6].onclick = &backButtonClick;


	menu.panels[index].btns[7].x = menu.panels[index].x + 100;
	menu.panels[index].btns[7].y = menu.panels[index].y + 240;
	menu.panels[index].btns[7].width = 50;
	menu.panels[index].btns[7].height = 30;

	menu.panels[index].btns[7].isVisible = 1;
	menu.panels[index].btns[7].hovered = 0;
	menu.panels[index].btns[7].mouseDown = 0;

	menu.panels[index].btns[7].text = L"Apply";
	menu.panels[index].btns[7].onclick = &appLyButtonClick;




	menu.panels[index].draw = &drawSettingsPanel;


	if(menu.state == 2) menu.panels[index].isActive = 1;
	else menu.panels[index].isActive = 0;
}

void getSettingsMessagePanel(struct Menu menu, int index) {
	menu.panels[index].width = 150;
	menu.panels[index].height = 150;
	menu.panels[index].x = menu.x + (menu.width - 150) / 2;
	menu.panels[index].y = menu.y + (menu.height- 150) / 2;

	menu.panels[index].length = 1;
	menu.panels[index].btns = (struct Button*)malloc(sizeof(struct Button) * 1);

	menu.panels[index].btns[0].width = 50;
	menu.panels[index].btns[0].height = 30;
	menu.panels[index].btns[0].x = menu.panels[index].x + 50;
	menu.panels[index].btns[0].y = menu.panels[index].y + 105;

	menu.panels[index].btns[0].text = L"OK";
	menu.panels[index].btns[0].isVisible = 1;
	menu.panels[index].btns[0].hovered = 0;
	menu.panels[index].btns[0].mouseDown = 0;

	menu.panels[index].btns[0].onclick = &settingsMessageBtnClick;

	menu.panels[index].draw = &drawSettingsMessagePanel;

	if (menu.state == 3) menu.panels[index].isActive = 1;
	else menu.panels[index].isActive = 0;
}

void getGameOverPanel(struct Menu menu, int index) {
	menu.panels[index].width = 200;
	menu.panels[index].height = 150;
	menu.panels[index].x = menu.x + (menu.width - 200) / 2;
	menu.panels[index].y = menu.y + (menu.height - 150) / 2;

	menu.panels[index].length = 1;
	menu.panels[index].btns = (struct Button*)malloc(sizeof(struct Button) * 1);

	menu.panels[index].btns[0].width = 60;
	menu.panels[index].btns[0].height = 30;
	menu.panels[index].btns[0].x = menu.panels[index].x + 70;
	menu.panels[index].btns[0].y = menu.panels[index].y + 105;

	menu.panels[index].btns[0].text = L"Restart";
	menu.panels[index].btns[0].isVisible = 1;
	menu.panels[index].btns[0].hovered = 0;
	menu.panels[index].btns[0].mouseDown = 0;

	menu.panels[index].btns[0].onclick = &restartBtnClick;

	menu.panels[index].draw = &drawGameOverPanel;

	if (menu.state == 4) menu.panels[index].isActive = 1;
	else menu.panels[index].isActive = 0;
}

void getFameHallPanel(struct Menu menu, int index) {
	menu.panels[index].width = 200;
	menu.panels[index].height = 330;
	menu.panels[index].x = menu.x + (menu.width - 200) / 2;
	menu.panels[index].y = menu.y + (menu.height - 330) / 2;

	menu.panels[index].length = 1;
	menu.panels[index].btns = (struct Button*)malloc(sizeof(struct Button) * 1);

	menu.panels[index].btns[0].width = 60;
	menu.panels[index].btns[0].height = 30;
	menu.panels[index].btns[0].x = menu.panels[index].x + 70;
	menu.panels[index].btns[0].y = menu.panels[index].y + 285;

	menu.panels[index].btns[0].text = L"Back";
	menu.panels[index].btns[0].isVisible = 1;
	menu.panels[index].btns[0].hovered = 0;
	menu.panels[index].btns[0].mouseDown = 0;

	menu.panels[index].btns[0].onclick = &backButtonClick;

	menu.panels[index].draw = &drawFameHallPanel;

	if (menu.state == 4) menu.panels[index].isActive = 1;
	else menu.panels[index].isActive = 0;
}