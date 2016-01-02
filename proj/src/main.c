#include <stdio.h>
#include <time.h>

#include "DuckHunt.h"
#include "utilities.h"

int main() {
	sef_startup();
	srand(time(NULL));

	InterruptVariables* iv = (InterruptVariables*) malloc(sizeof(InterruptVariables));

	int returnValue = initializeGame(iv);
	if(returnValue != 0)
		return returnValue;

	videoGraphicsInit(0x117);

	int menuChoice = 0;
	do{
		menuChoice = menu(iv);
		switch(menuChoice){
		case 0:
			score = playGame(iv);
			break;
		}
	}while(menuChoice != 2);

	exitGame();
	videoGraphicsExit();
	return 0;
}
