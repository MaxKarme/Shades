#pragma once
#include "BlockColor.h"
#include "framework.h"

struct Delta {
	struct BlockColor color;
	RECT coords;
};

struct Animation {
	RECT coords;
	struct BlockColor color;
	int id, afterId;
	int steps;
	int isDone, isActive;
	struct Delta delta;
};

void animationsTick();
void initAnimation(struct Animation* a);

void performParallelÖøåð(struct Animation a, int id);
void performAfter(struct Animation a, int id);

COLORREF getColorByAnimationId(int id);
RECT getCoordsByAnimationId(int id);
int isDone(int id);
int isActive(int id);
void deleteAnimation(int id);

int getId();
struct BlockColor getDeltaColor(struct BlockColor from, struct BlockColor to, int steps);
RECT getDeltaCoords(RECT from, RECT to, int steps);
