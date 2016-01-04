#ifndef __SCORES_H_
#define __SCORES_H_

#include "DuckHunt.h"

/**
 * @brief returns the place where a score belongs, if isn't top10, returns -1
 */
int scorePlace(int score);

/**
 * @brief inserts a score line on the line of the file where it belongs, updating the file accordingly
 */
void insertScore(char name[5], int score, int line);

/**
 * @brief prints the highscores information (top10)
 */
void printHighScores(InterruptVariables* iv);

#endif /* __SCORES_H_ */
