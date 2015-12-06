#include <stdio.h>
#include <minix/drivers.h>

#include "bitmap.h"

int main() {
	sef_startup();

	videoGraphicsInit(0x117);

	Bitmap* background = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/background-117.bmp");
	drawBitmap(background, 0, 0, ALIGN_LEFT);
	Bitmap* duck = loadBitmap("/home/lcom/lcom1516-t2g02/proj/res/images/duckgbkg.bmp");
	drawTransparentBitmap(duck, 100, 100, ALIGN_LEFT);
	flipBuffer();

	sleep(5);
	videoGraphicsExit();


}
