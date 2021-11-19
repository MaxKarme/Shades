#include "settings.h";

struct Settings settings;

void initSettings() {
	FILE* file;
	fopen_s(&file, "settings.txt", "rt");

	int collsCount, currentTheme;

	if (!file) return;

	fscanf_s(file, "%d", &collsCount);
	fscanf_s(file, "%d", &currentTheme);

	settings.collsCount = collsCount;
	settings.currentTheme = currentTheme;

	settings.changedCollsCount = collsCount;
	settings.changedTheme = currentTheme;

	fclose(file);
}

void saveSettings() {
	FILE* file;
	fopen_s(&file, "settings.txt", "wt");

	if (!file) return;

	fprintf_s(file, "%d ", settings.collsCount);
	fprintf_s(file, "%d", settings.currentTheme);

	fclose(file);
}

int getCollsCount() { return settings.collsCount; };
int getTheme() { return settings.currentTheme; };

int getChangedCollsCount() { return settings.changedCollsCount; };
int getChangedTheme() { return settings.changedTheme; };

int changeCollsCountSetting(int newCount) {
	settings.changedCollsCount = newCount;
}

int changeThemeSetting(int newTheme) {
	settings.changedTheme = newTheme;
}

int applyChanges() { // возвращает значение, указывающее стоит ли выводить окно с сообщением
	int res = 0;

	if (settings.collsCount != settings.changedCollsCount) res = 1;

	settings.collsCount = settings.changedCollsCount;
	settings.currentTheme = settings.changedTheme;

	changeFieldTheme(settings.currentTheme);

	saveSettings();

	return res;
}

int wasSettingsChanges() {
	if (settings.collsCount != settings.changedCollsCount) return 1;
	if (settings.currentTheme != settings.changedTheme) return 1;

	return 0;
}