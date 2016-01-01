#include <stdio.h>
#include <time.h>
#include <minix/drivers.h>

#include "bitmap.h"
#include "videoGraphics.h"
#include "GameInterface.h"
#include "Mouse.h"
#include "Timer.h"
#include "utilities.h"
#include "AnimSprite.h"
#include "Duck.h"
#include "Keyboard.h"

#define DUCKLIFETIME 450
#define DEATHTIMER 120

void drawScreen(Bitmap* background, Bitmap* foreground, Bitmap* scoreboard,
		unsigned int score, Duck* duck, int numDucks) {
	drawBitmap(background, 0, 0, ALIGN_LEFT);
	if (numDucks && duck->state != DEAD)
		drawDuck(*duck);
	drawTransparentBitmap(foreground, 0, 234, ALIGN_LEFT, 0);
	drawTransparentBitmap(scoreboard, getHRes() / 2 + 200, 700, ALIGN_LEFT, 0);
	drawNumber(score, getHRes() / 4 * 3 + 140, 704, 22);
}

unsigned int calculateScore(Duck* duck, int duckLifeTime) {
	float totalVelocity = abs(duck->xVel) + abs(duck->yVel);
	return 25 + duckLifeTime / 18 + (totalVelocity - 4) * 10;
}

int main() {
	sef_startup();
	srand(time(NULL));

	FILE * logfd;

	//logfd = fopen("/home/lcom/lcom1516-t2g02/proj/log.txt", "w");

	/*unsigned char konamiCode[10] = { UP, UP, DOWN, DOWN, LEFT, RIGHT, LEFT,
	 RIGHT, B, A };
	 unsigned char userKonami[10];*/

	videoGraphicsInit(0x117);

	Bitmap* background = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/bluebackground.bmp");
	Bitmap* foreground = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/frontbackground.bmp");
	Bitmap* fail = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/flyaway.bmp");
	Bitmap* scoreboard = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/score.bmp");

	AnimSprite* downDuck = createAnimSprite("upduck", 4);
	AnimSprite* upDuck = createAnimSprite("downduck", 4);
	AnimSprite* deadDuck = createAnimSprite("deadduck", 2);
	AnimSprite* flyingAwayDuck = createAnimSprite("awayduck", 4);
	AnimSprite* duckSprites[3];
	duckSprites[0] = downDuck;
	duckSprites[1] = upDuck;
	duckSprites[2] = deadDuck;
	duckSprites[3] = flyingAwayDuck;

	Duck* duck = (Duck*) malloc(sizeof(Duck));
	createDuck(duck, duckSprites);
	prepareDuck(duck, 0);

	message msg;
	int r, ipc_status;

	char packet[3];
	int returnValue, i = 0, j = 0;
	int leftButtonFlag = 0, failCount = 0, exit = 0, forceExit = 0;
	int duckLifeTime = DUCKLIFETIME, deathTimer = DEATHTIMER;
	unsigned int timeCounter = 0;
	unsigned int score = 0;

	int mouseSet = subscribeMouseInt();
	int timerSet = subscribeTimerInt();

	//int keyboardSet = subscribeKeyboardInt();
	if (mouseSet == -1) {
		return -1;
	}
	returnValue = enableSendingDataPackets();
	if (returnValue != 0) {
		return returnValue;
	}

	drawMouse();
	flipMouseBuffer();
	while ((failCount < 3 || exit != 1) && forceExit == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouseSet) { /* subscribed interrupt */
					returnValue = getPacket(&packet[i % 3]);
					if (returnValue == -1
							|| ((i % 3 == 0) && (packet[i % 3] & BIT(3) == 0)))
						continue;
					if (returnValue != 0) {
						return returnValue;
					}
					if ((i % 3) == 2) {
						updateMousePosition(packet);
						drawMouse();
						flipMouseBuffer();
						if ((packet[0] & BIT(0)) != 0 && leftButtonFlag == 0) {
							leftButtonFlag = 1;
							//if (konamiComplete == 0) {
							if (isHit(*duck) && duck->state != DYING
									&& duck->state != DEAD
									&& duck->state != FLYING_AWAY) {
								score = calculateScore(duck, duckLifeTime);
								getHit(duck);
								drawScreen(background, foreground, scoreboard,
										score, duck, 1);
							}
							//}
						}
						if ((packet[0] & BIT(0)) == 0)
							leftButtonFlag = 0;
						if ((packet[0] & BIT(1)) != 0) {
							forceExit = 1;
							break;
						}
					}
					i++;
				}
				if (msg.NOTIFY_ARG & timerSet) {
					switch (duck->state) {
					case 0:
					case 1:
						if (duckLifeTime > 0) {
							duckLifeTime--;
						} else {
							duck->state = FLYING_AWAY;
							duck->xVel = 0;
							duck->yVel = -3.5;
							duckLifeTime = DUCKLIFETIME;
							failCount++;
						}
						break;
					case 2:
						if (duck->duckSprites[duck->state]->cur_fig == 1)
							duck->yVel = 3;
						duck->yVel = duck->yVel * 1.5;
						if (isDead(duck))
							deathTimer = DEATHTIMER;
						break;
					case 3:
						if ((int) duck->y <= 4) {
							duck->state = DEAD;
							deathTimer = DEATHTIMER;
							if (failCount > 2)
								exit = 1;
						}
						break;
					case 4:
						if (deathTimer == 0) {
							duckLifeTime = DUCKLIFETIME;
							prepareDuck(duck, timeCounter);
						} else
							deathTimer--;
						break;
					}
					drawScreen(background, foreground, scoreboard, score, duck,
							1);
					if (duck->state == FLYING_AWAY)
						drawTransparentBitmap(fail, getHRes() / 2, 50,
								ALIGN_CENTER, 0);
					updateDuckPosition(duck);
					drawMouse();
					flipMouseBuffer();
					timeCounter++;
				}
				/*if (msg.NOTIFY_ARG & keyboardSet) {
				 unsigned char key;
				 kbdReadKey(&key);
				 if (key == 0xE0)
				 kbdReadKey(&key);
				 /*else if (key == konamiCode[konamiIndex]) {
				 userKonami[konamiIndex] = key;
				 konamiIndex++;
				 if (konamiIndex == 10) {
				 j = 0;
				 konamiIndex = 0;
				 konamiComplete = 350;
				 duck->y = getVRes();
				 duckSide = (duckSide + 1) % 2;
				 if (duckSide)
				 duck->x = getHRes();
				 else
				 duck->x = 0;
				 }
				 }
				 }*/
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else {
		}
	}
	disableStreamMode();
	unsubscribeMouseInt();
	unsubscribeTimerInt();
	//unsubscribeKeyboardInt();
	videoGraphicsExit();
	return score;

	/*Bitmap* duck = loadBitmap("/home/lcom/lcom1516-t2g02/proj/res/images/newduck.bmp");
	 drawTransparentBitmap(duck, 50, 50, ALIGN_LEFT);
	 flipBuffer();*/

	/*Bitmap* cursor = loadBitmap("/home/lcom/lcom1516-t2g02/proj/res/images/cursorpointerx2size.bmp");

	 FILE * logfd;

	 logfd = fopen("/home/lcom/lcom1516-t2g02/proj/log.txt", "w");

	 char* video_mem = getVideoMem();
	 int i;
	 for(i=0; i < duck->bitmapInfoHeader.imageSize; i++){
	 char test = (char) cursor->bitmapData[i];
	 fprintf(logfd, "%d\n", test);
	 }

	 fclose(logfd);*/

	//sleep(5);
	//videoGraphicsExit();
}
