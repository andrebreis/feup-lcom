#include "Scores.h"
#include "RTC.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

#define SCORES_FILE_PATH "/home/lcom/lcom1516-t2g02/proj/scores.txt"

// Para um determinado score decide onde inserir o score
int scorePlace(int score){

	// Declarar e abrir ficheiro
	FILE* scoresFile = NULL;
	scoresFile = fopen(SCORES_FILE_PATH, "r");
	if(scoresFile == NULL){
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
	while (fgets(buffer, sizeof(buffer), scoresFile) != NULL){
		// Vai buscar todo o texto ate ao espaço (nome)
		char *dump = strtok(buffer, " ");
		// Vai buscar todo o texto ate ao proximo espaço (score)
		char *line_score = strtok(NULL, " ");
		// Transforma para int
		int line_score_int = atoi(line_score);
		// Compara com o score a inserir
		if( score > line_score_int ){
			// Guarda index e sai
			index = i;
			break;
		}
		i++;
	}
	// Fecha o ficheiro
	fclose(scoresFile);

	if( i < 10 )
		index = i;

	// Retorna o index
	return index;
}

// Inserir uma linha na linha X de um ficheiro
void insertScore(char name[5], int score, int line){

	FILE *fhighscores;
	fhighscores = fopen(SCORES_FILE_PATH, "r");

	char buffer[BUFFER_SIZE];
	int i = 0;

	char **highscores = (char **)malloc(sizeof(char*) * 10);
	for(i=0; i<10; i++){
		highscores[i] = (char *)calloc(BUFFER_SIZE, sizeof(char));
	}

	i = 0;
	while (fgets(buffer, sizeof(buffer), fhighscores) != NULL){
		strcpy(highscores[i], buffer);
		i++;
	}

	fclose(fhighscores);

	for(i=9; i>line; i--){
		strcpy(highscores[i], highscores[i-1]);
	}

	Date scoreDate = getDate();
	char new_score[BUFFER_SIZE];
	strcpy(new_score, name);
	strcat(new_score, " ");
	strcat(new_score, itoa(score));
	strcat(new_score, " ");
	strcat(new_score, dateToStr(scoreDate));
	strcat(new_score, "\n");
	strcpy(highscores[line], new_score);

	fhighscores = fopen(SCORES_FILE_PATH, "w");
	for(i=0; i<10; i++){
		fputs(highscores[i], fhighscores);
	}

	fclose(fhighscores);
}
