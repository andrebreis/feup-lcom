#include "Scores.h"
#include "RTC.h"
#include "Keyboard.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

#define SCORES_FILE_PATH "/home//DuckHuntResources/res/scores.txt"

int scorePlace(int score) {

	FILE* scoresFile = NULL;
	scoresFile = fopen(SCORES_FILE_PATH, "r");
	if (scoresFile == NULL) {
		scoresFile = fopen(SCORES_FILE_PATH, "w");
		fclose(scoresFile);
		scoresFile = fopen(SCORES_FILE_PATH, "r");
	}

	int i = 0;
	int index = -1;

	char buffer[BUFFER_SIZE];
	while (fgets(buffer, sizeof(buffer), scoresFile) != NULL) {
		char *dump = strtok(buffer, " ");
		char *lineScore = strtok(NULL, " ");
		int lineScoreInt = atoi(lineScore);
		if (score > lineScoreInt) {
			index = i;
			break;
		}
		i++;
	}
	fclose(scoresFile);

	if (i < 10)
		index = i;

	return index;
}

void insertScore(char name[5], int score, int line) {

	FILE *fhighscores;
	fhighscores = fopen(SCORES_FILE_PATH, "r");

	char buffer[BUFFER_SIZE];
	int i = 0;

	char **highscores = (char **) malloc(sizeof(char*) * 10);
	for (i = 0; i < 10; i++) {
		highscores[i] = (char *) calloc(BUFFER_SIZE, sizeof(char));
	}

	i = 0;
	while (fgets(buffer, sizeof(buffer), fhighscores) != NULL) {
		strcpy(highscores[i], buffer);
		i++;
	}

	fclose(fhighscores);

	for (i = 9; i > line; i--) {
		strcpy(highscores[i], highscores[i - 1]);
	}

	Date scoreDate = getDate();
	char newScore[BUFFER_SIZE];
	strcpy(newScore, name);
	strcat(newScore, " ");
	strcat(newScore, itoa(score));
	strcat(newScore, " ");
	strcat(newScore, dateToStr(scoreDate));
	strcat(newScore, "\n");
	strcpy(highscores[line], newScore);

	fhighscores = fopen(SCORES_FILE_PATH, "w");
	for (i = 0; i < 10; i++) {
		fputs(highscores[i], fhighscores);
	}

	fclose(fhighscores);
	for (i = 0; i < 10; i++) {
		free(highscores[i]);
	}
	free(highscores);
}

void printHighScores(InterruptVariables* iv) {
	int i;

	Bitmap* highscoresBackground = loadBitmap(
			"/home//DuckHuntResources/res/images/highscores.bmp");
	drawBitmap(highscoresBackground, 0, 0, ALIGN_LEFT);

	FILE *fhighscores;
	fhighscores = fopen(SCORES_FILE_PATH, "r");

	char buffer[BUFFER_SIZE];

	char **highscores = (char **) malloc(sizeof(char*) * 10);
	for (i = 0; i < 10; i++) {
		highscores[i] = (char *) calloc(BUFFER_SIZE, sizeof(char));
	}

	i = 0;
	while (fgets(buffer, sizeof(buffer), fhighscores) != NULL) {
		strcpy(highscores[i], buffer);
		i++;
	}

	drawString("NR NAME SCORE HOUR DATE", 5, 140, 2);

	for (i = 0; i < 10; i++) {
		if (highscores[i][0] == '\0')
			continue;
		drawNumber(i+1, 50, 205 + i * 50, 5);
		drawString(highscores[i], 75, 190 + i*50, 2);
	}

	flipBuffer();
	unsigned char key = 0;
	char packet[3];
	while (key != ESC_KEY) {
		if ((iv->r = driver_receive(ANY, &iv->msg, &iv->ipcStatus)) != 0) {
			printf("driver_receive failed with: %d", iv->r);
			continue;
		}
		if (is_ipc_notify(iv->ipcStatus)) {
			switch (_ENDPOINT_P(iv->msg.m_source)) {
			case HARDWARE:
				if (iv->msg.NOTIFY_ARG & iv->mouseSet) {
					int returnValue = getPacket(&packet[i % 3]);
					if (returnValue == -1
							|| ((i % 3 == 0) && (packet[i % 3] & BIT(3) == 0)))
						continue;
					if ((i % 3) == 2) {
						updateMousePosition(packet);
					}
				}
				if (iv->msg.NOTIFY_ARG & iv->keyboardSet) {
					kbdReadKey(&key);
					if (key == 0xE0)
						kbdReadKey(&key);
				}
			}
		}
	}
	fclose(fhighscores);
	for (i = 0; i < 10; i++) {
		free(highscores[i]);
	}
	free(highscores);
}
