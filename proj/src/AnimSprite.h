#ifndef __ANIMSPRITE_H
#define __ANIMSPRITE_H

#include "bitmap.h"

/**
 * @struct AnimSprite
 * @brief contains an array of pointers to bitmap and info about when to update which bitmap is being shown
 */
typedef struct {
	int aspeed;     // no. frames per pixmap
	int cur_aspeed; // no. frames left to next change
	int num_fig;    // number of bitmaps
	int cur_fig;    // current bitmap
	Bitmap** maps;     // array of pointers to pixmaps
} AnimSprite;

/**
 * @brief creates an animated sprite and gets images from imgName and index
 */
AnimSprite* createAnimSprite(char* imgName, int startingImage , int noImages);

/**
 * @brief updates the AnimSprite info (figure being shown, how many frames until change,...)
 */
void updateAnimSprite(AnimSprite* asp);

/**
 * @brief draws an animsprite on the given position, and if inverted is != 0, draws it inverted on the x axis
 */
void drawAnimSprite(AnimSprite* asp, int x, int y, int inverted);

#endif /** __ANIMDPRITE_H **/
