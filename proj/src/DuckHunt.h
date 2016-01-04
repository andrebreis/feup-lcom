#ifndef __DUCKHUNT_H_
#define __DUCKHUNT_H_

#include <minix/drivers.h>
#include "bitmap.h"
#include "Duck.h"

#define DUCKLIFETIME 480
#define DEATHTIMER 120

typedef struct{
	message msg;
	int ipcStatus, r;
	int timerSet, keyboardSet, mouseSet;
}InterruptVariables;

int menu(InterruptVariables* iv);

void drawScreen(Bitmap* background, Bitmap* foreground, Bitmap* scoreboard,
		unsigned int score, Duck* duck, int numDucks, int numFails);

unsigned int calculateScore(Duck* duck, int duckLifeTime);

int calculateLifetime(int timeCounter);

int initializeGame(InterruptVariables* iv);

unsigned int playGame(InterruptVariables* iv);

char* showGameOver(InterruptVariables* iv, int score);

void exitGame();

#endif /*__DUCKHUNT_H_*/
