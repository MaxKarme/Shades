#include "menu.h"
#include "field.h"

struct Menu menu;

void initMenu(int x, int y, int width, int height) {
	menu.x = x;
	menu.y = y;
	menu.width = width;
	menu.height = height;

	initField(x, y + 50, width, height - 50);

	menu.state = 0;
	menu.length = 3;

	menu.panels = (struct Panel*)malloc(sizeof(struct Panel) * menu.length);

	getTopPanel(menu, 0);
	getPausePanel(menu, 1);
	getSettingsPanel(menu, 2);
}

void drawMenu(HDC hdc) {
	for (int i = 0; i < menu.length; ++i) {
		menu.panels[i].draw(hdc, menu, menu.panels[i]);
	}
}

int getMenuState() {
	return menu.state;
}

int inRect(struct Button* btn, int x1, int y1) {
	int res = 1;

	if (x1 < btn->x || x1 > btn->x + btn->width) res = 0;
	if (y1 < btn->y || y1 > btn->y + btn->height) res = 0;

	return res;
}

int MenuOnMouseMove(int x, int y) {
	int needInvalidate = 0;

	for (int i = 0; i < menu.length; ++i) {
		for (int j = 0; j < menu.panels[i].length; ++j) {
			struct Button btn = menu.panels[i].btns[j];

			int hovered = inRect(&btn, x, y);
			if (hovered != btn.hovered) needInvalidate = 1;

			menu.panels[i].btns[j].hovered = hovered;
			//if(menu.panels[i].btns[j].mouseDown) menu.panels[i].btns[j].mouseDown = hovered;
		}
	}

	return needInvalidate;
}

int MenuOnMouseDown(int x, int y) {
	int needInvalidate = 0;

	for (int i = 0; i < menu.length; ++i) {
		for (int j = 0; j < menu.panels[i].length; ++j) {
			struct Button btn = menu.panels[i].btns[j];

			int mouseDown = inRect(&btn, x, y);
			if (mouseDown != btn.mouseDown) needInvalidate = 1;

			menu.panels[i].btns[j].mouseDown = mouseDown;
		}
	}

	return needInvalidate;
}

int MenuOnMouseUp() {
	for (int i = 0; i < menu.length; ++i) {
		for (int j = 0; j < menu.panels[i].length; ++j) {
			struct Button btn = menu.panels[i].btns[j];
			if (!btn.mouseDown) continue;

			menu.panels[i].btns[j].onclick();
			menu.panels[i].btns[j].mouseDown = 0;

			return 1;
		}
	}

	return 0;
}

void changeState(int newState) {
	menu.state = newState;
}