#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define KEYBOARD_IRQ 	1
#define DELAY_US    	20000

#define UP 				0xC8
#define DOWN 			0xD0
#define LEFT			0xCB
#define RIGHT			0xCD
#define A				0x9E
#define B				0xB0

int subscribeKeyboardInt();

int unsubscribeKeyboardInt();

int kbdReadKey(unsigned char* resultKey);

#endif /* __KEYBOARD_H */
