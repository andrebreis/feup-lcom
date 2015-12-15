#include <stdio.h>
#include <time.h>
#include <minix/drivers.h>

#include "bitmap.h"
#include "videoGraphics.h"
#include "GameInterface.h"
#include "Mouse.h"
#include "Timer.h"
#include "utilities.h"
#include "Duck.h"
#include "Keyboard.h"

#define DUCK_VEL_X 3
#define DUCK_VEL_Y 3

int main() {
	sef_startup();
	srand(time(NULL));

	unsigned char konamiCode[10] = { UP, UP, DOWN, DOWN, LEFT, RIGHT, LEFT,
	RIGHT, B, A };
	unsigned char userKonami[10];

	videoGraphicsInit(0x117);

	Bitmap* konamiImages[] =
			{ loadBitmap(
					"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI0.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI1.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI2.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI3.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI4.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI5.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI6.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI7.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI8.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI9.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI10.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI11.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI12.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI13.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI14.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI15.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/KONAMI16.bmp") };

	/*AnimSprite* konami = createBigAnimSprite(konamiImages);
	konami->x = getHRes() / 2 - 70;
	konami->y = getVRes() / 2 - 70;*/

	Bitmap* background = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/bluebackground.bmp");
	drawBitmap(background, 0, 0, ALIGN_LEFT);
	Bitmap* frontground = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/frontbackground.bmp");
	drawTransparentBitmapTargetBuffer(frontground, 0, 234, ALIGN_LEFT,
			getBuffer());

	Bitmap* duckSprite[4] =
			{ loadBitmap(
					"/home/lcom/lcom1516-t2g02/proj/res/images/duck186x80.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/duck286x80.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/duck386x80.bmp"),
					loadBitmap(
							"/home/lcom/lcom1516-t2g02/proj/res/images/duck286x80.bmp") };

	AnimSprite* duck = createAnimSprite(duckSprite);
	duck->x = 0;
	duck->y = getVRes();
	message msg;
	int r, ipc_status;
	char packet[3];
	int returnValue, i = 0;
	int leftButtonFlag = 0, failCount = 0, duckSide = 0, konamiComplete = 0;
	int mouseSet = subscribeMouseInt();
	int timerSet = subscribeTimerInt();
	int keyboardSet = subscribeKeyboardInt();
	int konamiIndex = 0;
	int j = 0;
	unsigned char key;

	if (mouseSet == -1) {
		return -1;
	}
	returnValue = enableSendingDataPackets();
	if (returnValue != 0) {
		return returnValue;
	}

	drawMouse();
	flipMouseBuffer();
	while (failCount < 3) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouseSet) { /* subscribed interrupt */
					returnValue = getPacket(&packet[i % 3]);
					if (returnValue == -1)
						continue;
					if (returnValue != 0) {
						return returnValue;
					}
					if ((i % 3 == 0) && (packet[i % 3] & BIT(3) == 0)) {
						continue;
					}
					if ((i % 3) == 2) {
						updateMousePosition(packet);
						drawMouse();
						flipMouseBuffer();
						if ((packet[0] & BIT(0)) != 0 && leftButtonFlag == 0) {
							leftButtonFlag = 1;
							if (konamiComplete == 0) {
								if (isHit(duck)) {
									drawBitmap(background, 0, 0, ALIGN_LEFT);
									drawTransparentBitmapTargetBuffer(
											frontground, 0, 234, ALIGN_LEFT,
											getBuffer());
									duckSide = (duckSide + 1) % 2;
									if (duckSide)
										duck->x = getHRes();
									else
										duck->x = 0;
									duck->y = getVRes();
								}
							}
						}
						if ((packet[0] & BIT(0)) == 0)
							leftButtonFlag = 0;
						if ((packet[0] & BIT(1)) != 0) {
							disableStreamMode();
							unsubscribeMouseInt();
							unsubscribeTimerInt();
							unsubscribeKeyboardInt();
							videoGraphicsExit();
							return 0;
						}
					}
					i++;
				}
				if (msg.NOTIFY_ARG & timerSet) {
					if (konamiComplete == 0) {
						if (duck->y > 0) {
							drawBitmap(background, 0, 0, ALIGN_LEFT);
							if (duckSide == 0) {
								drawAnimSprite(duck);
								duck->x += DUCK_VEL_X;
								duck->y -= DUCK_VEL_Y;
							} else {
								drawInvertedAnimSprite(duck);
								duck->x -= DUCK_VEL_X;
								duck->y -= DUCK_VEL_Y;
							}
							drawTransparentBitmapTargetBuffer(frontground, 0,
									234, ALIGN_LEFT, getBuffer());
							drawMouse();
							flipMouseBuffer();
						} else {
							drawBitmap(background, 0, 0, ALIGN_LEFT);
							drawTransparentBitmapTargetBuffer(frontground, 0,
									234, ALIGN_LEFT, getBuffer());
							duck->y = getVRes();
							duckSide = (duckSide + 1) % 2;
							if (duckSide)
								duck->x = getHRes();
							else
								duck->x = 0;
							failCount++;
						}
					} else {
						drawBitmap(background, 0, 0, ALIGN_LEFT);
						drawTransparentBitmapTargetBuffer(konamiImages[(j/3)%18], getHRes()/2-70, getVRes()/2-70, ALIGN_LEFT, getBuffer());
						drawMouse();
						flipMouseBuffer();
						j++;
						konamiComplete--;
					}
				}
				if (msg.NOTIFY_ARG & keyboardSet) {
					kbdReadKey(&key);
					if (key == 0xE0)
						kbdReadKey(&key);
					else if (key == konamiCode[konamiIndex]) {
						userKonami[konamiIndex] = key;
						konamiIndex++;
						if (konamiIndex == 10) {
							j = 0;
							konamiIndex = 0;
							konamiComplete = 720;
							duck->y = getVRes();
							duckSide = (duckSide + 1) % 2;
							if (duckSide)
								duck->x = getHRes();
							else
								duck->x = 0;
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
	disableStreamMode();
	unsubscribeMouseInt();
	unsubscribeTimerInt();
	unsubscribeKeyboardInt();
	videoGraphicsExit();
	return 0;

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
