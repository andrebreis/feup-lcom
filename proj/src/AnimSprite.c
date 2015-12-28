#include "AnimSprite.h"
#include "videoGraphics.h"
#include "Mouse.h"

#define IMG_PATH "/home/lcom/lcom1516-t2g02/proj/res/images/"

AnimSprite* createAnimSprite(char* imgName, int noImages) {
	AnimSprite* asp = (AnimSprite*) malloc(sizeof(AnimSprite));
	int i;
	asp->maps = (Bitmap**) malloc(sizeof(Bitmap*) * noImages);
	for (i = 0; i < noImages; i++) {
		char path[1024];
		strcpy(path, IMG_PATH);
		strcat(path, imgName);
		strcat(path, itoa(i));
		strcat(path, ".bmp");
		asp->maps[i] = loadBitmap(path);
	}
	asp->num_fig = noImages;
	asp->cur_fig = 0;
	asp->aspeed = 3;
	asp->cur_aspeed = asp->aspeed;
	return asp;
}

void updateAnimSprite(AnimSprite* asp) {
	asp->cur_aspeed--;
	if (asp->cur_aspeed == 0) {
		asp->cur_fig = (asp->cur_fig + 1) % asp->num_fig;
		asp->cur_aspeed = asp->aspeed;
	}
}

void drawAnimSprite(AnimSprite* asp, int x, int y, int inverted) {
	drawTransparentBitmap(asp->maps[asp->cur_fig], x, y,
			ALIGN_LEFT, inverted);
}

/*int isHit(AnimSprite* asp) {
	Mouse* mouse = getMouse();
	Bitmap* currentBitmap = asp->maps[asp->cur_fig];
	int width = currentBitmap->bitmapInfoHeader.width, height =
			currentBitmap->bitmapInfoHeader.height;
	if (mouse->middleX >= asp->x && mouse->middleX <= asp->x + width
			&& mouse->middleY >= asp->y && mouse->middleY <= asp->y + height) {
		int firstPixelPosition = (mouse->middleY - asp->y) * width * 2
				+ (mouse->middleX - asp->x) * 2; //2 -> bytesperpixel
		if (currentBitmap->bitmapData[firstPixelPosition] != 116
				|| currentBitmap->bitmapData[firstPixelPosition + 2] != 116
				|| currentBitmap->bitmapData[firstPixelPosition + width * 2]
						!= 116
				|| currentBitmap->bitmapData[firstPixelPosition + width * 2 + 2]
						!= 116)
			return 1;

	}
	return 0;
}*/
