#ifndef __DUCKHUNT_H_
#define __DUCKHUNT_H_

#include <minix/drivers.h>
#include "bitmap.h"
#include "Duck.h"

#define DUCKLIFETIME 450
#define DEATHTIMER 120

typedef struct{
	message msg;
	int ipcStatus, r;
	int timerSet, keyboardSet, mouseSet;
}InterruptVariables;

int menu(InterruptVariables* iv);

void drawScreen(Bitmap* background, Bitmap* foreground, Bitmap* scoreboard,
		unsigned int score, Duck* duck, int numDucks);

unsigned int calculateScore(Duck* duck, int duckLifeTime);

int initializeGame(InterruptVariables* iv);

unsigned int playGame(InterruptVariables* iv);

void exitGame();

#endif /*__DUCKHUNT_H_*/
