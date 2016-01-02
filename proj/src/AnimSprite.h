#ifndef __ANIMSPRITE_H
#define __ANIMSPRITE_H

#include "bitmap.h"

typedef struct {
	int aspeed;     // no. frames per pixmap
	int cur_aspeed; // no. frames left to next change
	int num_fig;    // number of bitmaps
	int cur_fig;    // current bitmap
	Bitmap** maps;     // array of pointers to pixmaps
} AnimSprite;

AnimSprite* createAnimSprite(char* imgName, int startingImage , int noImages);

void updateAnimSprite(AnimSprite* asp);

void drawAnimSprite(AnimSprite* asp, int x, int y, int inverted);

//int isHit(AnimSprite* asp);

#endif /** __ANIMDPRITE_H **/
