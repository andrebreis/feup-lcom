#include <stdio.h>

#include "videoGraphics.h"
#include "GameInterface.h"
#include "Mouse.h"

Bitmap* drawBackground(){
	Bitmap* background = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/background-117.bmp");
	drawBitmap(background, 0, 0, ALIGN_LEFT);
	return background;
}

void drawMouse(){
	memcpy(getMouseBuffer(), getBuffer(), getVideoMemSize());
	drawTransparentBitmapTargetBuffer(getMouse()->icon, getMouse()->cornerX, getMouse()->cornerY, ALIGN_LEFT, getMouseBuffer());
}
