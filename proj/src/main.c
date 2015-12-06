#include <stdio.h>
#include <minix/drivers.h>

#include "bitmap.h"

int main() {
	sef_startup();

	videoGraphicsInit(0x114);

	//Bitmap* background = loadBitmap(
	//		"/home/lcom/lcom1516-t2g02/proj/res/images/background-117.bmp");
	//drawTransparentBitmap(background, 0, 0, ALIGN_LEFT);
	//drawBitmap(background, 0, 0, ALIGN_LEFT);
	Bitmap* duck = loadBitmap("/home/lcom/lcom1516-t2g02/proj/res/images/duck.bmp");
	drawTransparentBitmap(duck, 0, 0, ALIGN_LEFT);
	flipBuffer();
	FILE * logfd;
	logfd = fopen("/home/lcom/lcom1516-t2g02/proj/log.txt", "w");
	char* video_mem = getVideoMem();
	fprintf(logfd, "%d\n", video_mem[0]);
	fclose(logfd);
	sleep(5);
	videoGraphicsExit();


}
