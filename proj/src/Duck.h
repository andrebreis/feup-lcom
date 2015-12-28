#ifndef __DUCK_H
#define __DUCK_H

#include "AnimSprite.h"

typedef enum {
	UP = 0, DOWN = 1, DYING = 2, DEAD = 3
} duckState;

/*
 * mode:
 * 0 - from bottom left to top right:
 * xi e [-86, getHRes()/2-43]
 * yi = getVRes()
 *
 * 1 - from bottom right to top left:
 * xi e [getHRes()/2-43, getHRes()+86]
 * yi = getVRes()
 *
 * 2 - from left to right
 * xi = -86
 * yi e [230, 1024]
 *
 * 3 - from right to left
 * xi = getHRes()+86
 * yi e [230, 1024]
 */

typedef struct {
	int mode;
	duckState state;
	AnimSprite* duckSprites[3];
	int x, y, xVel, yVel;
} Duck;

void createDuck(Duck* duck, AnimSprite* duckSprites[3]);

void initializeDuck(Duck* duck);

void keepDuckOnScreen(Duck* duck);

void updateDuckPosition(Duck* duck);

void setXVel(Duck* duck, int vel);

void setYVel(Duck* duck, int vel);

void drawDuck(Duck duck);

int isHit(Duck duck);

#endif /* __DUCK_H */
