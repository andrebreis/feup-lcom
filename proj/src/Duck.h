#ifndef __DUCK_H
#define __DUCK_H

enum duckState {
	UP = 0, DOWN = 1, DEAD = 2
};

/*
 * mode:
 * 0 - from bottom left to top right:
 * xi e [-86, getHRes()/2]
 * yi = getVRes()
 *
 * 1 - from bottom right to top left:
 * xi e [getHRes()/2, getHRes()+86]
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

struct Duck {
	AnimSprite* duckSprites[3];
	duckState state;
	int mode;
	int x, y, xVel, yVel;
}

#endif /* __DUCK_H */
