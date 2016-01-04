#ifndef __DUCKHUNT_H_
#define __DUCKHUNT_H_

#include <minix/drivers.h>
#include "bitmap.h"
#include "Duck.h"

#define DUCKLIFETIME 480
#define DEATHTIMER 120

/**
 * @struct InterruptVariables
 * @brief Keeps the variables needed to deal with interruptions
 */
typedef struct{
	message msg;
	int ipcStatus, r;
	int timerSet, keyboardSet, mouseSet;
}InterruptVariables;

/**
 * @brief shows the menu and deals with the user clicks on its buttons (different return values for each button)
 */
int menu(InterruptVariables* iv);

/**
 * @brief draws the screen during a normal game cycle
 */
void drawScreen(Bitmap* background, Bitmap* foreground, Bitmap* scoreboard,
		unsigned int score, Duck* duck, int numDucks, int numFails);

/**
 * @brief calculates the score of a duck hit
 */
unsigned int calculateScore(Duck* duck, int duckLifeTime);

/**
 * @brief calculates the lifetime of the duck (based on the time passed)
 */
int calculateLifetime(int timeCounter);

/**
 * @brief subscribes the needed interrupts for the game
 */
int initializeGame(InterruptVariables* iv);

/**
 * @brief makes the game playable for the user and returns the score when the user loses
 */
unsigned int playGame(InterruptVariables* iv);

/**
 * @brief shows the game over screen and asks for name input for scores-keeping
 */
char* showGameOver(InterruptVariables* iv, int score);

/**
 * @brief unsubscribes the needed interrupts for the game
 */
void exitGame();

#endif /*__DUCKHUNT_H_*/
