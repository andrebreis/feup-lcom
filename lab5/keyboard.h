#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define DELAY_US    20000

#define OUT_BUF 	0x60
#define STAT_REG    0x64
#define KB_IRQ 		1
#define PAR_ERR 	BIT(7)
#define TO_ERR 		BIT(6)
#define OBF 		BIT(0)
#define IBF 		BIT(1)
#define ESC			0x81

int sendCommandtoKBC(unsigned long cmd);

int kbdReadKey(unsigned char* resultKey);

#endif /* __KEYBOARD_H */
