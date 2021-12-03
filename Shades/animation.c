#include "animation.h"

int animationsCount = 0;
#define maxLength 10

struct Animation animations[maxLength];

void initAnimation(struct Animation* a) {
	RECT initCoords = { 0, 0, 0, 0 };
	struct BlockColor initColor = { 0, 0, 0 };

	a->coords = initCoords;
	a->color = initColor;
	a->id = -1; 
	a->afterId = -1;

	a->steps = 0;

	a->isDone = 0;
	a->isActive = 0;

	a->delta.coords = initCoords;
	a->delta.color = initColor;
}

void deleteAnimation(int id) {
	int index = 0;
	while (id != animations[index].id) index++;

	for (int i = index; i < animationsCount - 1; ++i) {
		animations[i] = animations[i + 1];
	}

	animationsCount--;
}

void animationsTick() {
	for (int i = 0; i < animationsCount; ++i) {
		if (!animations[i].isActive) continue;
		if (animations[i].isDone) continue;

		animations[i].color.r += animations[i].delta.color.r;
		animations[i].color.g += animations[i].delta.color.g;
		animations[i].color.b += animations[i].delta.color.b;

		animations[i].coords.left += animations[i].delta.coords.left;
		animations[i].coords.top += animations[i].delta.coords.top;
		animations[i].coords.right += animations[i].delta.coords.right;
		animations[i].coords.bottom += animations[i].delta.coords.bottom;

		animations[i].steps--;

		if (animations[i].steps) continue;

		animations[i].isDone = 1;

		for (int j = 0; j < animationsCount; ++j) {
			if (animations[j].afterId == animations[i].id) animations[j].isActive = 1;
		}
	}
}

void performParallelWith(struct Animation a, int id) {
	int index = 0;
	while (animations[index].id != id) index++;

	a.afterId = animations[index].afterId;
	animations[animationsCount++] = a;
}

void performAfter(struct Animation a, int id) {
	a.afterId = id;
	animations[animationsCount++] = a;
}

void performNow(struct Animation a) {
	a.isActive = 1;
	animations[animationsCount++] = a;
}

COLORREF getColorByAnimationId(int id) {
	for (int i = 0; i < animationsCount; ++i) {
		if (animations[i].id == id) {
			int r = animations[i].color.r;
			int g = animations[i].color.g;
			int b = animations[i].color.b;
			return RGB(r, g, b);
		}
	}
}

RECT getCoordsByAnimationId(int id) {
	for (int i = 0; i < animationsCount; ++i) {
		if (animations[i].id == id) return animations[i].coords;
	}
}

int isDone(int id) {
	for (int i = 0; i < animationsCount; ++i) {
		if (animations[i].id == id) return animations[i].isDone;
	}

	return 1;
}

int isActive(int id) {
	for (int i = 0; i < animationsCount; ++i) {
		if (animations[i].id == id) return animations[i].isActive;
	}
}

int getId() {
	static int id = 1;
	return id++;
}

struct BlockColor getDeltaColor(struct BlockColor from, struct BlockColor to, int steps) {
	struct BlockColor res = { (to.r - from.r) / steps, (to.g - from.g) / steps, (to.b - from.b) / steps };
	return res;
}

RECT getDeltaCoords(RECT from, RECT to, int steps) {
	RECT res = { (to.left - from.left) / steps, (to.top - from.top) / steps, (to.right - from.right) / steps, (to.bottom - from.bottom) / steps };

}
