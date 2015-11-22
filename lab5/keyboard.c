#include <minix/drivers.h>

#include "keyboard.h"
#include "utilities.h"

int sendCommandtoKBC(unsigned long cmd){
	unsigned long stat;
	int returnValue;
	int i = 0;
	while( i < 10 ) {
		returnValue = sys_inb(STAT_REG, &stat);
		if(returnValue != 0)
			return returnValue;
		/* loop while 8042 input buffer is not empty */
		if( (stat & IBF) == 0 ) {
			returnValue = sys_outb(OUT_BUF, cmd); /*no args command*/
			if(returnValue != 0)
				return returnValue;
			else
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
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

