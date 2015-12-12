#include <stdio.h>
#include <minix/drivers.h>

#include "videoGraphics.h"
#include "GameInterface.h"
#include "Mouse.h"
#include "utilities.h"

int main() {
	sef_startup();

	videoGraphicsInit(0x117);

	Bitmap* background = loadBitmap(
			"/home/lcom/lcom1516-t2g02/proj/res/images/background-117.bmp");
	drawBitmap(background, 0, 0, ALIGN_LEFT);

	message msg;
	int r, ipc_status;
	char packet[3];
	int returnValue, i = 0, mouseSet;
	mouseSet = subscribeMouseInt();

	long int mouseX = 0, mouseY =  0;
	if (mouseSet == -1) {
		return -1;
	}
	returnValue = enableSendingDataPackets();
	if (returnValue != 0) {
		return returnValue;
	}

	drawMouse(mouseX, mouseY);
	flipMouseBuffer();
	while (1) {
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
						updateMousePosition(packet, &mouseX, &mouseY);
						drawMouse(mouseX, mouseY);
						flipMouseBuffer();
						if (packet[0] & BIT(1) != 0) {
							disableStreamMode();
							unsubscribeMouseInt();
							videoGraphicsExit();
							return 0;
						}
					}
					i++;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else {
		}
	}

	/*Bitmap* duck = loadBitmap("/home/lcom/lcom1516-t2g02/proj/res/images/newduck.bmp");
	 drawTransparentBitmap(duck, 50, 50, ALIGN_LEFT);
	 flipBuffer();*/

	/*FILE * logfd;

	 logfd = fopen("/home/lcom/lcom1516-t2g02/proj/log.txt", "w");

	 char* video_mem = getVideoMem();
	 int i;
	 for(i=0; i < duck->bitmapInfoHeader.imageSize; i++){
	 char test = (char) duck->bitmapData[i];
	 fprintf(logfd, "%d\n", test);
	 }

	 fclose(logfd);*/

	//sleep(5);
	//videoGraphicsExit();

}
