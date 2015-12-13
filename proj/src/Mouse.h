#ifndef __MOUSE_H
#define __MOUSE_H

#include "bitmap.h"

#define DELAY_US    20000

#define OUT_BUF 	0x60
#define STAT_REG    0x64
#define KBC_CMD_REG 0x64
#define PAR_ERR 	BIT(7)
#define TO_ERR 		BIT(6)
#define OBF 		BIT(0)
#define IBF 		BIT(1)
#define SET_COMMAND 0xED

#define MOUSE_IRQ		12
#define WRITE_TO_MOUSE 	0xD4
#define ACK				0xFA
#define NACK			0xFE
#define ERROR 			0xFC
#define RESEND 			0xFE
#define STAT_REQ		0xE9
#define SET_STREAM_MODE	0xEA
#define DIS_STREAM_MODE 0xF5
#define ENA_DATA_PACKS  0xF4

typedef struct{
	int cornerX, cornerY;
	int middleX, middleY;
	int middleSize;
	Bitmap* icon;
}Mouse;

int subscribeMouseInt();

int unsubscribeMouseInt();

int readFromKBC(char* readValue);

int writeToMouse(unsigned long cmd);

int enableStreamMode();

int disableStreamMode();

int enableSendingDataPackets();

int getPacket(char* packet);

void updateMousePosition(char packet[3], long int* x, long int* y);

#endif /* __MOUSE_H */
