#include <stdio.h>
#include <minix/drivers.h>

#include "bitmap.h"

int main() {
	sef_startup();

	videoGraphicsInit(0x114);

	Bitmap* background = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/background-117.bmp");
	drawBitmap(background, 0, 0, ALIGN_LEFT);

	printf("%d x %d\n", background->bitmapInfoHeader.width,
			background->bitmapInfoHeader.height);

	flipBuffer();
	sleep(5);

	videoGraphicsExit();
}
