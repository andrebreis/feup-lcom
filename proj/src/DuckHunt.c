#include "DuckHunt.h"

#include "utilities.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Font.h"

int initializeGame(InterruptVariables* iv) {
	iv->timerSet = subscribeTimerInt();
	iv->keyboardSet = subscribeKeyboardInt();
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
		unsigned int score, Duck* duck, int numDucks, int numFails) {
	Bitmap* lifeBox = loadBitmap(
			"/home//DuckHuntResources/res/images/lifeBox.bmp");
	Bitmap* lifeDuck = loadBitmap(
			"/home//DuckHuntResources/res/images/life0.bmp");
	Bitmap* noLifeDuck = loadBitmap(
			"/home//DuckHuntResources/res/images/life1.bmp");
	drawBitmap(background, 0, 0, ALIGN_LEFT);
	if (numDucks && duck->state != DEAD)
		drawDuck(*duck);
	drawTransparentBitmap(foreground, 0, 234, ALIGN_LEFT, 0);
	drawTransparentBitmap(scoreboard, getHRes() / 2 + 200, 700, ALIGN_LEFT, 0);
	drawNumber(score, getHRes() / 4 * 3 + 140, 704, 3);
	drawTransparentBitmap(lifeBox, getHRes() / 2 - 200, 700, ALIGN_LEFT, 0);
	int i;
	for (i = 1; i <= 3; i++) {
		if (i > numFails)
			drawTransparentBitmap(lifeDuck, getHRes() / 2 - 190 + 25 * i, 710,
					ALIGN_LEFT, 0);
		else
			drawTransparentBitmap(noLifeDuck, getHRes() / 2 - 190 + 25 * i, 710,
					ALIGN_LEFT, 0);
	}
	free(lifeBox);
	free(lifeDuck);
	free(noLifeDuck);
}

unsigned int calculateScore(Duck* duck, int duckLifeTime) {
	float totalVelocity = abs(duck->xVel) + abs(duck->yVel);
	return 25 + 25 / (abs(duckLifeTime - DUCKLIFETIME) + 1)
			+ (totalVelocity - 4) * 10;
}

int calculateLifetime(int timeCounter) {
	if (timeCounter < (60 * 60 * 2))
		return DUCKLIFETIME - timeCounter / (30 * 60);
	else
		return DUCKLIFETIME / 2;
}

int menu(InterruptVariables* iv) {
	Bitmap* menubackground = loadBitmap(
			"/home//DuckHuntResources/res/images/menubackground.bmp");
	drawBitmap(menubackground, 0, 0, ALIGN_LEFT);

	Mouse* mouse = getMouse();
	mouse->icon = loadBitmap(
			"/home//DuckHuntResources/res/images/arrowCursor.bmp");

	drawMouse();
	flipMouseBuffer();

	int i = 0;
	char packet[3];

	enableSendingDataPackets();
	sendCommandtoKBC(0x64, 0xF5);
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
							if (mouse->cornerX >= 200 && mouse->cornerX <= 816
									&& mouse->cornerY >= 402
									&& mouse->cornerY <= 524)
								return 0;

							if (mouse->cornerX >= 200 && mouse->cornerX <= 816
									&& mouse->cornerY >= 516
									&& mouse->cornerY <= 618)
								return 1;

							if (mouse->cornerX >= 200 && mouse->cornerX <= 816
									&& mouse->cornerY >= 626
									&& mouse->cornerY <= 708)
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
			"/home//DuckHuntResources/res/images/bluebackground.bmp");
	Bitmap* foreground = loadBitmap(
			"/home//DuckHuntResources/res/images/frontbackground.bmp");
	Bitmap* fail = loadBitmap(
			"/home//DuckHuntResources/res/images/flyaway.bmp");
	Bitmap* scoreboard = loadBitmap(
			"/home//DuckHuntResources/res/images/score.bmp");

	Mouse* mouse = getMouse();
	mouse->icon =
			loadBitmap(
					"/home//DuckHuntResources/res/images/cursorpointerx2size.bmp");

	AnimSprite* duckSprites[12];
	int m;
	for (m = 0; m <= 8; m += 4) {
		duckSprites[m] = createAnimSprite("upduck", m, 4);
		duckSprites[m + 1] = createAnimSprite("downduck", m, 4);
		duckSprites[m + 2] = createAnimSprite("deadduck", m / 2, 2);
		duckSprites[m + 3] = createAnimSprite("awayduck", m, 4);
	}

	unsigned char konamiCode[10] = { UP_KEY, UP_KEY, DOWN_KEY, DOWN_KEY,
	LEFT_KEY, RIGHT_KEY, LEFT_KEY, RIGHT_KEY, B_KEY, A_KEY };

	unsigned char userKonami[10];

	int konamiIndex = 0, konamiFlag = 0;
	char packet[3];
	int returnValue, i = 0, j = 0;
	int leftButtonFlag = 0, failCount = 0, exit = 0, forceExit = 0;
	int duckLifeTime = DUCKLIFETIME, deathTimer = DEATHTIMER;
	unsigned int timeCounter = 0;
	unsigned int score = 0;

	Duck* duck = (Duck*) malloc(sizeof(Duck));
	createDuck(duck, duckSprites);
	prepareDuck(duck, 0);

	drawScreen(background, foreground, scoreboard, score, duck, 1, 0);
	drawMouse();
	flipMouseBuffer();
	enableSendingDataPackets();
	sendCommandtoKBC(0x64, 0xF4);
	while (failCount < 3 || exit != 1) {
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
							if (isHit(*duck) && duck->state != DYING
									&& duck->state != DEAD
									&& duck->state != FLYING_AWAY) {
								score += calculateScore(duck, duckLifeTime);
								getHit(duck);
								drawScreen(background, foreground, scoreboard,
										score, duck, 1, failCount);
							}
						}
						if ((packet[0] & BIT(0)) == 0)
							leftButtonFlag = 0;
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
							failCount++;
						}
						break;
					case 2:
						if (duck->duckSprites[duck->state + 4 * duck->color]->cur_fig
								== 1 && duck->yVel == 0)
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
							duckLifeTime = calculateLifetime(timeCounter);
							prepareDuck(duck, timeCounter);
						} else
							deathTimer--;
						break;
					}
					drawScreen(background, foreground, scoreboard, score, duck,
							1, failCount);
					if (duck->state == FLYING_AWAY)
						drawTransparentBitmap(fail, getHRes() / 2, 50,
								ALIGN_CENTER, 0);
					updateDuckPosition(duck);
					drawMouse();
					flipMouseBuffer();
					timeCounter++;
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
					if (key == konamiCode[konamiIndex]) {
						userKonami[konamiIndex] = key;
						konamiIndex++;
						if (konamiIndex == 10) {
							konamiIndex = 0;
							konamiFlag = (konamiFlag + 1) % 3;
							AnimSprite* profSprites[12];
							if (konamiFlag) {
								if (konamiFlag == 1) {
									for (m = 0; m <= 8; m += 4) {
										profSprites[m] = createAnimSprite(
												"upprof", m, 4);
										profSprites[m + 1] = createAnimSprite(
												"downprof", m, 4);
										profSprites[m + 2] = createAnimSprite(
												"deadprof", m / 2, 2);
										profSprites[m + 3] = createAnimSprite(
												"awayprof", m, 4);
									}
								} else {
									for (m = 12; m <= 20; m += 4) {
										profSprites[m - 12] = createAnimSprite(
												"upprof", m, 4);
										profSprites[m + 1 - 12] =
												createAnimSprite("downprof", m,
														4);
										profSprites[m + 2 - 12] =
												createAnimSprite("deadprof",
														m / 2, 2);
										profSprites[m + 3 - 12] =
												createAnimSprite("awayprof", m,
														4);
									}
								}
								for (m = 0; m < 12; m++)
									duck->duckSprites[m] = profSprites[m];
							} else {
								for (m = 0; m < 12; m++)
									duck->duckSprites[m] = duckSprites[m];
							}
						}
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else {
		}
	}
	return score;
}

char* showGameOver(InterruptVariables* iv, int score) {
	Bitmap* gameOver = loadBitmap(
			"/home//DuckHuntResources/res/images/gameover.bmp");
	drawBitmap(gameOver, 0, 0, ALIGN_LEFT);

	flipBuffer();

	Mouse* mouse = getMouse();
	mouse->icon = loadBitmap(
			"/home//DuckHuntResources/res/images/arrowCursor.bmp");
	int letterCursorX = 69, letterCursorY = 559, cursorTimer = 60, cursorFlag =
			1;
	Bitmap* letterCursor = loadBitmap(
			"/home//DuckHuntResources/res/images/letterCursor.bmp");

	unsigned char key = 0;
	char* name = calloc(6, sizeof(char)), packet[3];
	int i = 0, j = 0;
	unsigned long stat;
	sys_inb(STAT_REG, &stat);
	while (stat & OBF) {
		kbdReadKey(&key);
		sys_inb(STAT_REG, &stat);
	}
	while (key != ENTER_KEY) {
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
				if (iv->msg.NOTIFY_ARG & iv->timerSet) {
					if (cursorTimer == 0) {
						cursorFlag = (cursorFlag + 1) % 2;
						cursorTimer = 60;
					}
					drawBitmap(gameOver, 0, 0, ALIGN_LEFT);
					drawString("SCORE", 100, 300, 5);
					drawNumber(score, 400, 305, 5);
					drawString("WRITE YOUR NAME", 42, 650, 3);
					drawString("AND PRESS ENTER", 42, 700, 3);
					if (cursorFlag == 1)
						drawTransparentBitmap(letterCursor, letterCursorX,
								letterCursorY, ALIGN_LEFT, 0);
					cursorTimer--;
					if (name[0] != "\0")
						drawString(name, 69, 559, 20);
					flipBuffer();
				}
				if (iv->msg.NOTIFY_ARG & iv->keyboardSet) {
					kbdReadKey(&key);
					char letter = codeToChar(key);
					if (key == 0xE0)
						kbdReadKey(&key);
					if (key == LEFT_KEY && j != 0) {
						j--;
						letterCursorX -= 53;
					}
					if (key == RIGHT_KEY && j != 4 && name[j + 1] != '\0') {
						j++;
						letterCursorX += 53;
					} else if (letter >= 'A' && letter <= 'Z') {
						name[j] = letter;
						if (j != 4) {
							j++;
							letterCursorX += 53;
						}
					}
					unsigned long stat;
					sys_inb(STAT_REG, &stat);
					while (stat & OBF) {
						kbdReadKey(&key);
						sys_inb(STAT_REG, &stat);
					}
				}
			}
		}
	}
	return name;
}

void exitGame() {
	disableStreamMode();
	unsubscribeMouseInt();
	unsigned long stat;
	char key;
	sys_inb(STAT_REG, &stat);
	while (stat & OBF) {
		kbdReadKey(&key);
		sys_inb(STAT_REG, &stat);
	}
	unsubscribeKeyboardInt();
	unsubscribeTimerInt();
}
