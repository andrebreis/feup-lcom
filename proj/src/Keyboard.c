#include <minix/drivers.h>

#include "Keyboard.h"
#include "utilities.h"

static int hook = KEYBOARD_IRQ;

int subscribeKeyboardInt(){
	return subscribeInt(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
}

int unsubscribeKeyboardInt(){
	return unsubscribeInt(&hook);
}

int kbdReadKey(unsigned char* resultKey){
	int returnValue;
	unsigned long stat, data;
	int i = 0;
	while( i < 10 ) {
		returnValue = sys_inb(STAT_REG, &stat);
		if(returnValue != 0)
			return returnValue;
		/*	loop while 8042 output buffer is empty*/
		if( stat & OBF ) {
			returnValue = sys_inb(OUT_BUF, &data);
			if(returnValue != 0)
				return returnValue;
			if ( (stat & (PAR_ERR | TO_ERR) ) == 0 ) {
				*resultKey = data;
				return 0;
			}
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
}
char codeToChar(unsigned long breakCode){
	char line0[10] = "qwertyuiop";
	char line1[9] = "asdfghjkl";
	char line2[7] = "zxcvbnm";
	if(breakCode >= 0x90 && breakCode <= 0x99){
		int diff = breakCode-0x90;
		return line0[(breakCode-0x90)];
	}
	else if(breakCode >= 0x9E && breakCode <= 0xA6)
		return line1[(breakCode-0x9E)];

	else if(breakCode >=0xAC && breakCode <= 0xB2)
		return line2[(breakCode-0xAC)];
}
