#pragma once
#include <stdio.h>
#include "field.h"


struct Settings {
	int collsCount;
	int currentTheme;
	int changedCollsCount;
	int changedTheme;
};

void initSettings();
int getCollsCount();
int getTheme();

int changeCollsCountSetting(int newCount);
int changeThemeSetting(int newTheme);

int wasSettingsChanges();
int applyChanges();