#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define KEYBOARD_IRQ 	1
#define DELAY_US    	20000

#define UP_KEY			0xC8
#define DOWN_KEY		0xD0
#define LEFT_KEY		0xCB
#define RIGHT_KEY		0xCD
#define A_KEY			0x9E
#define B_KEY			0xB0
#define ENTER_KEY		0x9C
#define ESC_KEY			0x81

int subscribeKeyboardInt();

int unsubscribeKeyboardInt();

int kbdReadKey(unsigned char* resultKey);

#endif /* __KEYBOARD_H */
