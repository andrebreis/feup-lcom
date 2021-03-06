#include "AnimSprite.h"
#include "videoGraphics.h"
#include "Mouse.h"

#define IMG_PATH "/home/DuckHuntResources/res/images/"

AnimSprite* createAnimSprite(char* imgName, int startingImage, int noImages) {
	AnimSprite* asp = (AnimSprite*) malloc(sizeof(AnimSprite));
	int i;
	asp->maps = (Bitmap**) malloc(sizeof(Bitmap*) * noImages);
	for (i = startingImage; i < startingImage + noImages; i++) {
		char path[1024];
		strcpy(path, IMG_PATH);
		strcat(path, imgName);
		strcat(path, itoa(i));
		strcat(path, ".bmp");
		asp->maps[i-startingImage] = loadBitmap(path);
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
	drawTransparentBitmap(asp->maps[asp->cur_fig], x, y, ALIGN_LEFT, inverted);
}
