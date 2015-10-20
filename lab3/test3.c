#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8042.h"
static int hook = KB_IRQ;

int sendCommandtoKBC(unsigned long cmd){
	unsigned long stat;
	int returnValue;
	while( 1 ) {
		returnValue = sys_inb(STAT_REG, &stat);
		if(returnValue != 0)
			return returnValue;
		/* loop while 8042 input buffer is not empty */
		if( (stat & IBF) == 0 ) {
			returnValue = sys_outb(KBC_CMD_REG, cmd); /*no args command*/
			if(returnValue != 0)
				return returnValue;
			else
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

int kbdReadKey(unsigned char* resultKey){
	int returnValue;
	unsigned long stat, data;
	while( 1 ) {
		returnValue = sys_inb(STAT_REG, &stat);
		if(returnValue != 0)
			return returnValue;
		/*	loop while 8042 output buffer is empty*/
		if( stat & OBF ) {
			returnValue = sys_inb(OUT_BUF, &data);
			if(returnValue != 0)
				return returnValue;
			if ( (stat &(PAR_ERR | TO_ERR)) == 0 ) {
				*resultKey = data;
				return 0;
			}
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

int irq_subscribe_int(void) {
	int bitmask = BIT(hook);
	if (sys_irqsetpolicy(KB_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook) == OK)
		if (sys_irqenable(&hook) == OK)
			return bitmask;
	return -1;
}

int irq_unsubscribe_int() {
	if (sys_irqdisable(&hook) == OK)
		if (sys_irqrmpolicy(&hook) == OK)
			return 0;
	return 1;

}

int kbd_test_scan(unsigned short ass) {
	unsigned char firstByte, secondByte;
	int returnValue;
	irq_subscribe_int();
	do{
		returnValue = kbdReadKey(&firstByte);
		if(returnValue != 0)
			return returnValue;
		if(firstByte == 0xE0){
			returnValue = kbdReadKey(&secondByte);
			if(returnValue != 0)
				return returnValue;
			if((secondByte & BIT(7)) == 0)
				printf("MakeCode: %x%x\n", firstByte, secondByte);
			else
				printf("BreakCode: %x%x\n", firstByte, secondByte);
		}
		else
			if((firstByte & BIT(7)) == 0)
				printf("MakeCode: %x\n", firstByte);
			else
				printf("BreakCode: %x\n", firstByte);
	}while(firstByte != 0x81);
	irq_unsubscribe_int();
	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *toggle){
	return 0;
}

int kbd_test_timed_scan(unsigned short n){
	return 0;
}

