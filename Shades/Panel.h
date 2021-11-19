#pragma once
#include "framework.h"
#include "Button.h"
#include "Menu.h"
#include "field.h"
#include "blockColor.h"
#include "settings.h"

struct Panel {
	int width, height, x, y;
	int length;
	struct Button* btns; // массив кнопок
	void (*draw)(HDC hdc, struct Menu* menu, struct Panel* panel);
	int isActive;
};

void getTopPanel(struct Menu menu, int index);
void getPausePanel(struct Menu menu, int index);
void getSettingsPanel(struct Menu menu, int index);
void getSettingsMessagePanel(struct Menu menu, int index);