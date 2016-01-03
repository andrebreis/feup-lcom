#include "DuckHunt.h"

#include "utilities.h"
#include "Mouse.h"
#include "Keyboard.h"

int initializeGame(InterruptVariables* iv) {
	iv->timerSet = subscribeTimerInt();
	//iv->keyboardSet = subscribeKeyboardInt();
	iv->mouseSet = subscribeMouseInt();

	/*int returnValue = enableSendingDataPackets();
	 if (returnValue != 0) {
	 return returnValue;
	 }*/

	if (iv->mouseSet == -1)
		return -1;
	if (iv->keyboardSet == -1)
		return -1;
	if (iv->timerSet == -1)
		return -1;

	//sendCommandtoKBC(0x64, 0xF5);

	return 0;
}

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

int menu(InterruptVariables* iv) {
	Bitmap* menubackground = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/menubackground.bmp");
	drawBitmap(menubackground, 0, 0, ALIGN_LEFT);

	Mouse* mouse = getMouse();
	mouse->icon = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/arrowCursor.bmp");

	drawMouse();
	flipMouseBuffer();

	int i = 0;
	char packet[3];

	enableSendingDataPackets();
	//sendCommandtoKBC(0x64, 0xF5);
	while (1) {
		if ((iv->r = driver_receive(ANY, &iv->msg, &iv->ipcStatus)) != 0) {
			printf("driver_receive failed with: %d", iv->r);
			continue;
		}
		if (is_ipc_notify(iv->ipcStatus)) { /* received notification */
			switch (_ENDPOINT_P(iv->msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (iv->msg.NOTIFY_ARG & iv->mouseSet) { /* subscribed interrupt */
					int returnValue = getPacket(&packet[i % 3]);
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

						if ((packet[0] & BIT(0)) != 0) {
							if (mouse->cornerX >= 250 && mouse->cornerX <= 778
									&& mouse->cornerY >= 428
									&& mouse->cornerY <= 508)
								return 0;

							if (mouse->cornerX >= 284 && mouse->cornerX <= 738
									&& mouse->cornerY >= 548
									&& mouse->cornerY <= 604)
								return 1;

							if (mouse->cornerX >= 468 && mouse->cornerX <= 560
									&& mouse->cornerY >= 650
									&& mouse->cornerY <= 686)
								return 2;
						}
						if ((packet[0] & BIT(1)) != 0) {
							return 2;
						}
					}
					i++;
				}
				if (iv->msg.NOTIFY_ARG & iv->keyboardSet) {
					unsigned char key;
					kbdReadKey(&key);
					if (key == 0xE0)
						kbdReadKey(&key);
					unsigned long stat;
					sys_inb(STAT_REG, &stat);
					while (stat & OBF) {
						kbdReadKey(&key);
						sys_inb(STAT_REG, &stat);
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else {
		}
	}
}

unsigned int playGame(InterruptVariables* iv) {

	Bitmap* background = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/bluebackground.bmp");
	Bitmap* foreground = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/frontbackground.bmp");
	Bitmap* fail = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/flyaway.bmp");
	Bitmap* scoreboard = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/score.bmp");

	Mouse* mouse = getMouse();
	mouse->icon =
			loadBitmap(
					"/home/lcom/lcom1516-t2g02/proj/res/images/cursorpointerx2size.bmp");

	AnimSprite* duckSprites[12];
	int m;
	for (m = 0; m <= 8; m += 4) {
		duckSprites[m] = createAnimSprite("upduck", m, 4);
		duckSprites[m+1] = createAnimSprite("downduck", m, 4);
		duckSprites[m+2] = createAnimSprite("deadduck", m/2, 2);
		duckSprites[m+3] = createAnimSprite("awayduck", m, 4);
	}

	char packet[3];
	int returnValue, i = 0, j = 0;
	int leftButtonFlag = 0, failCount = 0, exit = 0, forceExit = 0;
	int duckLifeTime = DUCKLIFETIME, deathTimer = DEATHTIMER;
	unsigned int timeCounter = 0;
	unsigned int score = 0;

	Duck* duck = (Duck*) malloc(sizeof(Duck));
	createDuck(duck, duckSprites);
	prepareDuck(duck, 0);

	drawScreen(background, foreground, scoreboard, score, duck, 1);
	drawMouse();
	flipMouseBuffer();
	enableSendingDataPackets();
	//sendCommandtoKBC(0x64, 0xF4);
	while ((failCount < 3 || exit != 1) && forceExit == 0) {
		if ((iv->r = driver_receive(ANY, &iv->msg, &iv->ipcStatus)) != 0) {
			printf("driver_receive failed with: %d", iv->r);
			continue;
		}
		if (is_ipc_notify(iv->ipcStatus)) { /* received notification */
			switch (_ENDPOINT_P(iv->msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (iv->msg.NOTIFY_ARG & iv->mouseSet) { /* subscribed interrupt */
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
								score += calculateScore(duck, duckLifeTime);
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
				if (iv->msg.NOTIFY_ARG & iv->timerSet) {
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
						if (duck->duckSprites[duck->state+4*duck->color]->cur_fig == 1 && duck->yVel == 0)
							duck->yVel = 3;
						duck->yVel = duck->yVel * 1.05;
						if (isDead(duck))
							deathTimer = DEATHTIMER;
						break;
					case 3:
						if ((int) duck->y <= 1) {
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
				/*if (iv->msg.NOTIFY_ARG & iv->keyboardSet) {
					unsigned char key;
					kbdReadKey(&key);
					if (key == 0xE0)
						kbdReadKey(&key);
					unsigned long stat;
					sys_inb(STAT_REG, &stat);
					while (stat & OBF) {
						kbdReadKey(&key);
						sys_inb(STAT_REG, &stat);
					}
				}*/
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
	return score;
}

void exitGame() {
	videoGraphicsExit();
	disableStreamMode();
	unsubscribeMouseInt();
	/*unsigned long stat;
	char key;
	sys_inb(STAT_REG, &stat);
	while (stat & OBF) {
		kbdReadKey(&key);
		sys_inb(STAT_REG, &stat);
	}
	unsubscribeKeyboardInt();*/
	unsubscribeTimerInt();
}

//logfd = fopen("/home/lcom/lcom1516-t2g02/proj/log.txt", "w");

/*unsigned char konamiCode[10] = { UP, UP, DOWN, DOWN, LEFT, RIGHT, LEFT,
 RIGHT, B, A };
 unsigned char userKonami[10];*/

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
