#include "Font.h"
#include "bitmap.h"

#define FONT_PATH "/home/lcom/lcom1516-t2g02/proj/res/images/font/"

void drawNumber(unsigned int number, int x, int y, int distance) {
	int i, digit;
	for (i = number; i != 0; i = i / 10) {
		char path[1024];
		strcpy(path, FONT_PATH);
		digit = i % 10;
		strcat(path, itoa(digit));
		strcat(path, ".bmp");
		Bitmap* temp = loadBitmap(path);
		drawTransparentBitmap(temp, x, y, ALIGN_RIGHT, 0);
		x -= distance;
	}
}

void drawString(char* string, int x, int y){
	int i;
	for(i = 0; string[i] != '\0' && string[i] != '\n'; i++){
		char path[1024];
		strcpy(path, FONT_PATH);
		strcat(path, string[i]);
		strcat(path, ".bmp");
		Bitmap* temp = loadBitmap(path);
		drawTransparentBitmap(temp, x, y, ALIGN_RIGHT, 0);
		x += 2;
	}
}
