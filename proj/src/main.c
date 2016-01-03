#include <stdio.h>
#include <time.h>
#include <string.h>

#include "DuckHunt.h"
#include "Scores.h"
#include "utilities.h"
#include "RTC.h"

#define FONT_PATH "/home/lcom/lcom1516-t2g02/proj/res/images/font/"

int main(){

	sef_startup();
	sys_enable_iop(SELF);
	srand(time(NULL));

	InterruptVariables* iv = (InterruptVariables*) malloc(sizeof(InterruptVariables));

	int returnValue = initializeGame(iv);
	if(returnValue != 0)
		return returnValue;

	videoGraphicsInit(0x117);

	int score = 0;
	int playerPlace;
	int menuChoice = 0;
	char* playerName = malloc(sizeof(char)*6);
	//playGame(iv);
	/*do{

		switch(menuChoice){
		case 0:
			score = playGame(iv);
			playerName = showGameOver(iv, score);
			int playerPlace = scorePlace(score);
			if(playerPlace != -1)
				insertScore(playerName, score, playerPlace);
			break;
		}
	}while(menuChoice != 2);*/
	menuChoice = menu(iv);
	playGame(iv);
	showGameOver(iv, 50);
	//sleep(2);
	exitGame();
	videoGraphicsExit();
	return 0;
}
