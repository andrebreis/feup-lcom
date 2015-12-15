#include <stdio.h>

#include "Duck.h"
#include "videoGraphics.h"
#include "Mouse.h"

AnimSprite* createAnimSprite(Bitmap* maps[4]) {
	AnimSprite* asp = (AnimSprite*) malloc(sizeof(AnimSprite));
	int i;
	for (i = 0; i < 4; i++) {
		asp->maps[i] = maps[i];
	}
	asp->num_fig = 4;
	asp->cur_fig = 0;
	asp->aspeed = 3;
	asp->cur_aspeed = asp->aspeed;
	return asp;
}

AnimSprite* createBigAnimSprite(Bitmap* maps[18]) {
	AnimSprite* asp = (AnimSprite*) malloc(sizeof(AnimSprite));
	int i;
	for (i = 0; i < 18; i++) {
		asp->maps[i] = maps[i];
	}
	asp->num_fig = 18;
	asp->cur_fig = 0;
	asp->aspeed = 2;
	asp->cur_aspeed = asp->aspeed;
	return asp;
}

void drawAnimSprite(AnimSprite* asp) {
	drawTransparentBitmapTargetBuffer(asp->maps[asp->cur_fig], asp->x, asp->y,
			ALIGN_LEFT, getBuffer());
	asp->cur_aspeed--;
	if (asp->cur_aspeed == 0) {
		asp->cur_fig = (asp->cur_fig + 1) % asp->num_fig;
		asp->cur_aspeed = asp->aspeed;
	}

}

void drawInvertedAnimSprite(AnimSprite* asp) {
	drawTransparentBitmapInverted(asp->maps[asp->cur_fig], asp->x, asp->y,
			ALIGN_LEFT, getBuffer());
	asp->cur_aspeed--;
	if (asp->cur_aspeed == 0) {
		asp->cur_fig = (asp->cur_fig + 1) % asp->num_fig;
		asp->cur_aspeed = asp->aspeed;
	}
}

int isHit(AnimSprite* asp) {
	Mouse* mouse = getMouse();
	Bitmap* currentBitmap = asp->maps[asp->cur_fig];
	int width = currentBitmap->bitmapInfoHeader.width, height =
			currentBitmap->bitmapInfoHeader.height;
	if (mouse->middleX >= asp->x && mouse->middleX <= asp->x + width
			&& mouse->middleY >= asp->y && mouse->middleY <= asp->y + height) {
		/*fprintf(logfd, "got here3\n");
		 int firstPixelPosition = mouse->middleY * width * 2
		 + mouse->middleX * 2; //2 -> bytesperpixel
		 if (currentBitmap->bitmapData[firstPixelPosition] != 116
		 || currentBitmap->bitmapData[firstPixelPosition + 2] != 116
		 || currentBitmap->bitmapData[firstPixelPosition + width * 2]
		 != 116
		 || currentBitmap->bitmapData[firstPixelPosition + width * 2 + 2]
		 != 116) */
		return 1;

	}
	return 0;
}
