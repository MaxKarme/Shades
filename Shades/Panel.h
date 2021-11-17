#pragma once
#include "framework.h"
#include "Button.h"
#include "Menu.h"

struct Panel {
	int width, height, x, y;
	int length;
	struct Button* btns; // массив кнопок
	void (*draw)(HDC hdc, struct Menu menu, struct Panel);
};

void getTopPanel(struct Menu menu, int index);
void getPausePanel(struct Menu menu, int index);
void getSettingsPanel(struct Menu menu, int index);