#include <minix/drivers.h>

#include "Mouse.h"
#include "videoGraphics.h"
#include "utilities.h"

static int hook = MOUSE_IRQ;

int subscribeMouseInt(){
	return subscribeInt(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
}

int unsubscribeMouseInt(){
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
		}
		else if(cmdFeedback == NACK)
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

void updateMousePosition(char packet[3], long int* x, long int* y){
	/*int xOVF = (packet[0] & BIT(6)), yOVF = (packet[0] & BIT(7));
	unsigned char maxDelta = -1;
	if(xOVF) *x += maxDelta;
	if(yOVF) *y += maxDelta;
	int deltaX = packet[1], deltaY = packet[2];

	if((packet[0] & BIT(5)) == 0)
	 *y -= deltaY;
	else
	 *y -= (deltaY | (-1 << 8));

	if((packet[0] & BIT(6)) == 0)
	 *x += deltaX;
	else
	 *x += (deltaX | (-1 << 8));

	if(*x >= getHRes()) *x = getHRes();
	if(*x <= 0) *x = 0;
	if(*y >= getVRes()) *y = getVRes();
	if(*y <= 0) *y = 1;*/

	int xOVF = (packet[0] & BIT(6)), yOVF = (packet[0] & BIT(7));
	unsigned char maxDelta = -1;
	int deltaX = packet[1], deltaY = packet[2];
	if(xOVF) deltaX += maxDelta;
	if(yOVF) deltaY += maxDelta;
	if((packet[0] & BIT(5)) != 0) deltaY = (deltaY | (-1 << 8));
	if((packet[0] & BIT(6)) != 0) deltaX = (deltaX | (-1 << 8));

	if(*x + deltaX >= getHRes())
		*x = getHRes();
	else if(*x + deltaX <= 0)
		*x = 1;
	else
		*x += deltaX;

	if(*y - deltaY >= getVRes())
		*y = getVRes();
	else if(*y - deltaY <= 0)
		*y = 1;
	else
		*y = *y - deltaY;

}
