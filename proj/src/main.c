#include <stdio.h>
#include <time.h>

#include "DuckHunt.h"
#include "Scores.h"
#include "utilities.h"
#include "RTC.h"

int main() {
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
	do{
		menuChoice = menu(iv);
		switch(menuChoice){
		case 0:
			score = playGame(iv);
			int playerPlace = scorePlace(score);
			if(playerPlace != -1)
				insertScore("ANDRE", score, playerPlace);
			break;
		}
	}while(menuChoice != 2);

	exitGame();
	videoGraphicsExit();
	return 0;
}
