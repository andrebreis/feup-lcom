#ifndef __DUCK_H
#define __DUCK_H

#include "bitmap.h"


typedef struct {
	int x, y;
	int aspeed;     // no. frames per pixmap
	int cur_aspeed; // no. frames left to next change
	int num_fig;    // number of bitmaps
	int cur_fig;    // current bitmap
	Bitmap** maps;     // array of pointers to pixmaps
} AnimSprite;

AnimSprite* createAnimSprite(Bitmap* maps[4]);

AnimSprite* createBigAnimSprite(Bitmap* maps[18]);

void drawAnimSprite(AnimSprite* asp);

int isHit(AnimSprite* asp);


#endif /* __DUCK_H */
