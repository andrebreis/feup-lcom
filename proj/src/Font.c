#include "Font.h"
#include "bitmap.h"

#include <stdio.h>
#include <string.h>

#define FONT_PATH "/home/DuckHuntResources/res/images/font/"

void drawNumber(unsigned int number, int x, int y, int distance) {
	int i, digit;
	if(number == 0){
		char path[1024];
		strcpy(path, FONT_PATH);
		strcat(path, itoa(number));
		strcat(path, ".bmp");
		Bitmap* temp = loadBitmap(path);
		drawTransparentBitmap(temp, x, y, ALIGN_RIGHT, 0);
		free(temp);
	}
	else{
		for (i = number; i != 0; i = i / 10) {
			char path[1024];
			strcpy(path, FONT_PATH);
			digit = i % 10;
			strcat(path, itoa(digit));
			strcat(path, ".bmp");
			Bitmap* temp = loadBitmap(path);
			drawTransparentBitmap(temp, x, y, ALIGN_RIGHT, 0);
			x = x - temp->bitmapInfoHeader.width - distance;
			free(temp);
		}
	}
}

void drawString(char* string, int x, int y, int distance){
	int i;
	for(i = 0; i < strlen(string); i++){
		if(string[i] == ' ')
			x += 32;
		else if(string[i] == '\n')
			continue;
		else if(string[i] == '/' || string[i] == ':'){
			char path[1024];
			if(string[i] == '/' )
				sprintf(path, "%s%s.bmp", FONT_PATH, "BAR");
			else
				sprintf(path, "%s%s.bmp", FONT_PATH, "TWOPOINT");
			Bitmap* temp = loadBitmap(path);
			drawTransparentBitmap(temp, x, y, ALIGN_LEFT, 0);
			x = x + temp->bitmapInfoHeader.width + distance;
			free(temp);
		}
		else{
			char path[1024];
			sprintf(path, "%s%c.bmp", FONT_PATH, string[i]);
			Bitmap* temp = loadBitmap(path);
			if(isdigit(string[i]))
				drawTransparentBitmap(temp, x, y+12, ALIGN_LEFT, 0);
			else
				drawTransparentBitmap(temp, x, y, ALIGN_LEFT, 0);
			x = x + temp->bitmapInfoHeader.width + distance;
			free(temp);
		}
	}
}
