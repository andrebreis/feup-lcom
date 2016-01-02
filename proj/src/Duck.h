#ifndef __DUCK_H
#define __DUCK_H

#include "AnimSprite.h"

typedef enum {
	UP = 0, DOWN = 1, DYING = 2, FLYING_AWAY = 3, DEAD = 4
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
	int mode, color;
	duckState state;
	AnimSprite* duckSprites[12];
	float x, y, xVel, yVel;
} Duck;

void createDuck(Duck* duck, AnimSprite* duckSprites[12]);

void initializeDuck(Duck* duck);

void keepDuckOnScreen(Duck* duck);

void updateDuckPosition(Duck* duck);

void setXVel(Duck* duck, float vel);

void setYVel(Duck* duck, float vel);

void setVelocity(Duck* duck, unsigned int timeCounter);

void drawDuck(Duck duck);

int isHit(Duck duck);

int getHit(Duck* duck);

int isDead(Duck* duck);

void prepareDuck(Duck* duck, unsigned int timeCounter);

#endif /* __DUCK_H */
