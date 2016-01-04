#include "Scores.h"
#include "RTC.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

#define SCORES_FILE_PATH "/home/lcom/lcom1516-t2g02/proj/scores.txt"

// Para um determinado score decide onde inserir o score
int scorePlace(int score) {

	// Declarar e abrir ficheiro
	FILE* scoresFile = NULL;
	scoresFile = fopen(SCORES_FILE_PATH, "r");
	if (scoresFile == NULL) {
		scoresFile = fopen(SCORES_FILE_PATH, "w");
		fclose(scoresFile);
		scoresFile = fopen(SCORES_FILE_PATH, "r");
	}

	// contador de linhas e index onde inserir a linha de score
	int i = 0;
	int index = -1;

	// Cria espaço para ler o ficheiro
	char buffer[BUFFER_SIZE];
	// Le ate ao final do ficheiro
	while (fgets(buffer, sizeof(buffer), scoresFile) != NULL) {
		// Vai buscar todo o texto ate ao espaço (nome)
		char *dump = strtok(buffer, " ");
		// Vai buscar todo o texto ate ao proximo espaço (score)
		char *lineScore = strtok(NULL, " ");
		// Transforma para int
		int lineScoreInt = atoi(lineScore);
		// Compara com o score a inserir
		if (score > lineScoreInt) {
			// Guarda index e sai
			index = i;
			break;
		}
		i++;
	}
	// Fecha o ficheiro
	fclose(scoresFile);

	if (i < 10)
		index = i;

	// Retorna o index
	return index;
}

// Inserir uma linha na linha X de um ficheiro
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
			"/home/lcom/lcom1516-t2g02/proj/res/images/highscores.bmp");
	drawBitmap(highscoresBackground, 0, 0, ALIGN_LEFT);

	char **highscores = (char **) malloc(sizeof(char*) * 10);
	for (i = 0; i < 10; i++) {
		highscores[i] = (char *) calloc(BUFFER_SIZE, sizeof(char));
	}

	while (fgets(buffer, sizeof(buffer), fhighscores) != NULL) {
		strcpy(highscores[i], buffer);
		i++;
	}

	for (i = 1; i <= 10; i++) {
		if (highscores[i][0] == '\0')
			break;
		drawNumber(i, 25, 50 + i * 50);
		drawString(highscores[i - 1]);
	}
	flipBuffer();

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
}
