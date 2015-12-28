#include "Duck.h"
#include "videoGraphics.h"
#include "Mouse.h"

#define BOTTOM_OF_SCREEN getVRes() - 210

void initializeDuck(Duck* duck) {
	int width = duck->duckSprites[0].maps[0]->bitmapInfoHeader.width;
	duck->mode = rand() % 4;
	switch (duck->mode) {
	case 0:
		duck->x = rand() % ((getHRes() + 3 * width) / 2) - width + 1;
		duck->y = BOTTOM_OF_SCREEN
		;
		duck->xVel = 1;
		duck->yVel = -1;
		duck->state = UP;
		break;
	case 1:
		duck->x = getHRes() - rand() % ((getHRes() + 3 * width) / 2) - width
				+ 1;
		duck->y = BOTTOM_OF_SCREEN
		;
		duck->xVel = -1;
		duck->yVel = -1;
		duck->state = UP;
		break;
	case 2:
		duck->x = 0;
		duck->y = rand() % BOTTOM_OF_SCREEN
		;
		duck->xVel = 1;
		duck->yVel = rand() % 2 * 2 - 1;
		duck->state = (duck->yVel == -1) ? UP : DOWN;
		break;
	case 3:
		duck->x = getHRes() + width - 1;
		duck->y = rand() % BOTTOM_OF_SCREEN
		;
		duck->xVel = -1;
		duck->yVel = rand() % 2 * 2 - 1;
		duck->state = (duck->yVel == -1) ? UP : DOWN;
		break;
	}
}

void createDuck(Duck* duck, AnimSprite duckSprites[3]) {
	int i;
	for(i = 0; i < 3; i++) duck->duckSprites[i] = duckSprites[i];
	initializeDuck(duck);
}

void keepDuckOnScreen(Duck* duck) {
	int width = duck->duckSprites[0].maps[0]->bitmapInfoHeader.width;
	int height = duck->duckSprites[0].maps[0]->bitmapInfoHeader.height;

	if (duck->x <= -width) {
		duck->x = -width + 1;
		duck->xVel = -duck->xVel;
	} else if (duck->x >= getHRes() + width) {
		duck->x = getHRes() + width - 1;
		duck->xVel = -duck->xVel;
	}

	if (duck->y <= -height) {
		duck->y = -height + 1;
		duck->yVel = -duck->yVel;
	} else if (duck->y >= BOTTOM_OF_SCREEN)
	{
		duck->y = BOTTOM_OF_SCREEN - 1;
		duck->yVel = -duck->yVel;
	}
}

void updateDuckPosition(Duck* duck) {
	if (duck->state == DEAD)
		return;
	duck->x += duck->xVel;
	duck->y += duck->yVel;
	updateAnimSprite(&duck->duckSprites[duck->state]);
	keepDuckOnScreen(duck);
}

void setXVel(Duck* duck, int vel){
	duck->xVel = duck->xVel*vel;
}

void setYVel(Duck* duck, int vel){
	duck->yVel = duck->yVel*vel;
}

void drawDuck(Duck duck){
	int inverted = (duck.xVel < 0);
	drawAnimSprite(duck.duckSprites[duck.state], duck.x, duck.y, inverted);
}

int isHit(const Duck duck) {
	if(duck.state != UP && duck.state != DOWN)
		return 0;
	Mouse* mouse = getMouse();
	AnimSprite asp = duck.duckSprites[duck.state];
	Bitmap* currentBitmap = asp.maps[asp.cur_fig];
	int width = currentBitmap->bitmapInfoHeader.width, height =
			currentBitmap->bitmapInfoHeader.height;
	if (mouse->middleX >= duck.x && mouse->middleX <= duck.x + width
			&& mouse->middleY >= duck.y && mouse->middleY <= duck.y + height) {
		int firstPixelPosition = (mouse->middleY - duck.y) * width * 2
				+ (mouse->middleX - duck.x) * 2; //2 -> bytesperpixel
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
