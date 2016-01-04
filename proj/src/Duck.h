#ifndef __DUCK_H
#define __DUCK_H

#include "AnimSprite.h"

/**
 * @enum duckState
 * @brief keeps the duck state (moving direction or purpose)
 */
typedef enum {
	UP = 0, DOWN = 1, DYING = 2, FLYING_AWAY = 3, DEAD = 4
} duckState;

/*
 * mode:
 * 0 - from bottom left to top right:
 * xi e [-86, getHRes()/2-43]
 * yi = getVRes()
 *
 * 1 - from bottom right to top left:
 * xi e [getHRes()/2-43, getHRes()+86]
 * yi = getVRes()
 *
 * 2 - from left to right
 * xi = -86
 * yi e [230, 1024]
 *
 * 3 - from right to left
 * xi = getHRes()+86
 * yi e [230, 1024]
 */

/**
 * @struct Duck
 * @brief keeps the duck information - its state and AnimSprites (Duck Images), position and velocity
 */
typedef struct {
	int mode, color;
	duckState state;
	AnimSprite* duckSprites[12];
	float x, y, xVel, yVel;
} Duck;

/**
 * @brief initializes duck parameters
 */
void initializeDuck(Duck* duck);

/**
 * @brief creates a duck with the given images, and initializes its parameters
 */
void createDuck(Duck* duck, AnimSprite* duckSprites[12]);

/**
 * @brief limits the Duck position to the screen borders
 */
void keepDuckOnScreen(Duck* duck);

/**
 * @brief updates the duck position in accord to its velocity and keeps it on the screen borders
 */
void updateDuckPosition(Duck* duck);

/**
 * @brief sets the duck x velocity to the given absolute value (keeps the velocity signal)
 */
void setXVel(Duck* duck, float vel);

/**
 * @brief sets the duck y velocity to the given absolute value (keeps the velocity signal)
 */
void setYVel(Duck* duck, float vel);

/**
 * @brief sets the duck x and y velocity, depending on the time passed since the beginning of the game
 */
void setVelocity(Duck* duck, unsigned int timeCounter);

/**
 * @brief draws the duck on its position
 */
void drawDuck(Duck duck);

/**
 * @brief returns 1 if the duck was shot, 0 if it wasn't
 */
int isHit(Duck duck);

/**
 * @brief actualizes the duck behaviour to get shot
 */
int getHit(Duck* duck);

/**
 * @brief returns 1 if the duck is dead or if it was dying and should now be dead, updating the state, 0 otherwise
 */
int isDead(Duck* duck);

/**
 * @brief prepares the duck values to a new game cycle
 */
void prepareDuck(Duck* duck, unsigned int timeCounter);

#endif /* __DUCK_H */
