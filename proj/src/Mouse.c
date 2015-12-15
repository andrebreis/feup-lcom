#include <minix/drivers.h>

#include "Mouse.h"
#include "videoGraphics.h"
#include "utilities.h"

static int hook = MOUSE_IRQ;

static Mouse* mouse = NULL;

Mouse* getMouse(){
	if(!mouse){
		mouse  = (Mouse*) malloc(sizeof(Mouse));
		mouse->icon = loadBitmap("/home/lcom/lcom1516-t2g02/proj/res/images/cursorpointerx2size.bmp");
		mouse->middleX = getHRes()/2;
		mouse->middleY = getVRes()/2;
		mouse->cornerX = mouse->middleX - mouse->icon->bitmapInfoHeader.width/2;
		mouse->cornerY = mouse->middleY - mouse->icon->bitmapInfoHeader.height/2;
		mouse->middleSize = 2;
	}
	return mouse;
}

int subscribeMouseInt() {
	return subscribeInt(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
}

int unsubscribeMouseInt() {
	return unsubscribeInt(&hook);
}

int readFromKBC(char* readValue) {
	int returnValue;
	unsigned long stat, data;
	int i = 0;
	while (i < 3) {
		returnValue = sys_inb(STAT_REG, &stat);
		if (returnValue != 0) {
			return returnValue;
		}
		if (stat & OBF) {
			returnValue = sys_inb(OUT_BUF, &data);
			if (returnValue != 0) {
				return returnValue;
			}
			if ((stat & (PAR_ERR | TO_ERR)) == 0) {
				*readValue = data;
				return 0;
			} else {
				return -1;
			}
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
	return -1;
}

int sendCommandtoKBC(unsigned long port, unsigned long cmd) {
	unsigned long stat;
	int returnValue;
	int i = 0;
	while (i < 10) {
		returnValue = sys_inb(STAT_REG, &stat);
		if (returnValue != 0)
			return returnValue;
		/* loop while 8042 input buffer is not empty */
		if ((stat & IBF) == 0) {
			returnValue = sys_outb(port, cmd); /*no args command*/
			if (returnValue != 0)
				return returnValue;
			else
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
}

int writeToMouse(unsigned long cmd) {
	int returnValue, goodFeedback = 0;
	unsigned long cmdFeedback;

	Mouse* mouse = getMouse();
	while (!goodFeedback) {

		returnValue = sendCommandtoKBC(STAT_REG, WRITE_TO_MOUSE);
		if (returnValue != 0)
			return returnValue;

		returnValue = sendCommandtoKBC(OUT_BUF, cmd);
		if (returnValue != 0)
			return returnValue;

		returnValue = sys_inb(OUT_BUF, &cmdFeedback);
		if (returnValue != 0)
			return returnValue;

		if (cmdFeedback == ACK)
			goodFeedback = TRUE;
		else if (cmdFeedback == ERROR) {
			printf("Error sending command!\n");
			return -1;
		} else if (cmdFeedback == NACK)
			cmd = RESEND;
	}
	return 0;
}

int enableStreamMode() {
	return writeToMouse(SET_STREAM_MODE);
}

int disableStreamMode() {
	return writeToMouse(DIS_STREAM_MODE);
}

int enableSendingDataPackets() {
	int returnValue;

	sendCommandtoKBC(STAT_REG, 0xA8);

	returnValue = disableStreamMode();
	if (returnValue != 0)
		return returnValue;

	returnValue = writeToMouse(ENA_DATA_PACKS);
	if (returnValue != 0)
		return returnValue;

	return enableStreamMode();
}

int getPacket(char* packet) {
	int returnValue;
	returnValue = readFromKBC(packet);
	return returnValue;
}

void updateMousePosition(char packet[3]) {
	/*int xOVF = (packet[0] & BIT(6)), yOVF = (packet[0] & BIT(7));
	 unsigned char maxDelta = -1;
	 if(xOVF) mouse->middleX += maxDelta;
	 if(yOVF) mouse->middleY += maxDelta;
	 int deltaX = packet[1], deltaY = packet[2];

	 if((packet[0] & BIT(5)) == 0)
	 mouse->middleY -= deltaY;
	 else
	 mouse->middleY -= (deltaY | (-1 << 8));

	 if((packet[0] & BIT(6)) == 0)
	 mouse->middleX += deltaX;
	 else
	 mouse->middleX += (deltaX | (-1 << 8));

	 if(mouse->middleX >= getHRes()) mouse->middleX = getHRes();
	 if(mouse->middleX <= 0) mouse->middleX = 0;
	 if(mouse->middleY >= getVRes()) mouse->middleY = getVRes();
	 if(mouse->middleY <= 0) mouse->middleY = 1;*/

	int xOVF = (packet[0] & BIT(6)), yOVF = (packet[0] & BIT(7));
	unsigned char maxDelta = -1;
	int deltaX = (unsigned char) packet[1], deltaY = (unsigned char) packet[2];
	/*if (xOVF)
		deltaX += maxDelta;
	if (yOVF)
		deltaY += maxDelta;*/
	if ((packet[0] & BIT(5)) != 0)
		deltaY = (deltaY | (-1 << 8));
	if ((packet[0] & BIT(4)) != 0)
		deltaX = (deltaX | (-1 << 8));

	if (mouse->middleX + deltaX >= getHRes())
		mouse->middleX = getHRes();
	else
		mouse->middleX += deltaX;

	if (mouse->middleY - deltaY >= getVRes())
		mouse->middleY = getVRes();
	else
		mouse->middleY = mouse->middleY - deltaY;

	mouse->cornerX = mouse->middleX - mouse->icon->bitmapInfoHeader.width/2;
	mouse->cornerY = mouse->middleY - mouse->icon->bitmapInfoHeader.height/2;
	if(mouse->cornerX < 0){
		mouse->cornerX = 0;
		mouse->middleX = mouse->icon->bitmapInfoHeader.width/2;
	}
	if(mouse->cornerY < 0){
		mouse->cornerY = 0;
		mouse->middleY = mouse->icon->bitmapInfoHeader.height/2;
	}
}
