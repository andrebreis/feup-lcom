#include "Duck.h"
#include "videoGraphics.h"
#include "Mouse.h"

#include <stdio.h>

#define BOTTOM_OF_SCREEN getVRes() - 210

void initializeDuck(Duck* duck) {
	int width = 86;
	duck->color = rand() % 3;
	duck->mode = rand() % 3;
	switch (duck->mode) {
	case 0:
		duck->x = rand() % getHRes() - width / 2;
		duck->y = BOTTOM_OF_SCREEN;
		duck->xVel = rand() % 2 * 2 - 1;
		duck->yVel = -1;
		duck->state = UP;
		break;
	case 1:
		duck->x = 0;
		duck->y = rand() % BOTTOM_OF_SCREEN;
		duck->xVel = 1;
		duck->yVel = rand() % 2 * 2 - 1;
		duck->state = (duck->yVel == -1) ? UP : DOWN;
		break;
	case 2:
		duck->x = getHRes() + width - 1;
		duck->y = rand() % BOTTOM_OF_SCREEN;
		duck->xVel = -1;
		duck->yVel = rand() % 2 * 2 - 1;
		duck->state = (duck->yVel == -1) ? UP : DOWN;
		break;
	}
}

void createDuck(Duck* duck, AnimSprite* duckSprites[12]) {
	int i;
	for (i = 0; i < 12; i++)
		duck->duckSprites[i] = duckSprites[i];
	initializeDuck(duck);
}

void keepDuckOnScreen(Duck* duck) {
	int width = 86;
	int height = 80;
	if (duck->x < 0) {
		duck->x = 0;
		if(duck->state < 2)
			duck->xVel = duck->xVel * (-1);
	}
	else if (duck->x + width/2 > getHRes()) {
		duck->x = getHRes() - width/2;
		if(duck->state < 2)
			duck->xVel = duck->xVel * (-1);
	}

	if (duck->y <= 0) {
		duck->y = 0;
		if(duck->state < 2){
			duck->yVel = duck->yVel * (-1);
			duck->state = DOWN;
		}
	} else if (duck->y > BOTTOM_OF_SCREEN) {
		duck->y = BOTTOM_OF_SCREEN - 1;
		if(duck->state < 2){
			duck->yVel = duck->yVel * (-1);
			duck->state = UP;
		}
	}
}

void updateDuckPosition(Duck* duck) {
	if (duck->state == DEAD)
		return;
	duck->x += duck->xVel;
	duck->y += duck->yVel;
	keepDuckOnScreen(duck);
	updateAnimSprite(duck->duckSprites[duck->state+4*duck->color]);
}

void setXVel(Duck* duck, float vel) {
	duck->xVel = duck->xVel * vel;
}

void setYVel(Duck* duck, float vel) {
	duck->yVel = duck->yVel * vel;
}

void setVelocity(Duck* duck, unsigned int timeCounter){
	float totalVelocity = 4 + (float) timeCounter/(60*20);
	setXVel(duck, 1 + rand() % (int)(totalVelocity -1));
	setYVel(duck, totalVelocity - abs(duck->xVel));
}

void drawDuck(Duck duck) {
	int inverted = (duck.xVel < 0);
	drawAnimSprite(duck.duckSprites[duck.state+4*duck.color], (int) duck.x, (int) duck.y, inverted);
}

int isHit(const Duck duck) {
	if (duck.state != UP && duck.state != DOWN)
		return 0;
	Mouse* mouse = getMouse();
	AnimSprite* asp = duck.duckSprites[duck.state+4*duck.color];
	Bitmap* currentBitmap = asp->maps[asp->cur_fig];
	int width = currentBitmap->bitmapInfoHeader.width, height =
			currentBitmap->bitmapInfoHeader.height;
	if (mouse->middleX >= duck.x && mouse->middleX <= duck.x + width
			&& mouse->middleY >= duck.y && mouse->middleY <= duck.y + height) {
		int firstPixelPosition = (mouse->middleY - duck.y) * width * 2
				+ (mouse->middleX - duck.x) * 2;
		if (currentBitmap->bitmapData[firstPixelPosition] != 116
				|| currentBitmap->bitmapData[firstPixelPosition + 2] != 116
				|| currentBitmap->bitmapData[firstPixelPosition + width * 2]
											 != 116
											 || currentBitmap->bitmapData[firstPixelPosition + width * 2 + 2]
																		  != 116)
			return 1;

	}
	return 0;
}

int getHit(Duck* duck){
	duck->state = DYING;
	duck->xVel = 0;
	duck->yVel = 0;
	duck->duckSprites[duck->state+4*duck->color]->cur_fig = 0;
	duck->duckSprites[duck->state+4*duck->color]->aspeed = 10000;
	duck->duckSprites[duck->state+4*duck->color]->cur_aspeed = 30;
}

int isDead(Duck* duck){
	if(duck->state == DYING && duck->y >= BOTTOM_OF_SCREEN - 10){
		duck->state = DEAD;
		return 1;
	}
	if(duck->state == DEAD)
		return 1;
	return 0;
}

void prepareDuck(Duck* duck, unsigned int timeCounter) {
	initializeDuck(duck);
	setVelocity(duck, timeCounter);
}
