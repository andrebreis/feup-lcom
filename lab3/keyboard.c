#include "keyboard.h"
#include "i8042.h"
#include  "timer.h"
#include <minix/drivers.h>
#include <minix/syslib.h>


static int hook = KB_IRQ;
static char ledsByte = 0;

int kbc_subscribe_int(void) {
	int bitmask = BIT(hook);
	if (sys_irqsetpolicy(KB_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook) == OK)
		if (sys_irqenable(&hook) == OK)
			return bitmask;
	return -1;
}

int kbc_unsubscribe_int() {
	if (sys_irqdisable(&hook) == OK)
		if (sys_irqrmpolicy(&hook) == OK)
			return 0;
	return 1;

}

int sendCommandtoKBC(unsigned long cmd){
	unsigned long stat;
	int returnValue;
	while( 1 ) {
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

int setLedsToDefault(){
	int returnValue = sendCommandtoKBC(SET_COMMAND);
	if(returnValue != 0)
		return -1;
	returnValue = sendCommandtoKBC(ledsByte);
	return returnValue;
}

int toggleLed(int led){
	int returnValue;
	if(led < 0 || led > 2)
		return -7;
	if(ledsByte & BIT(led) == 0)
		ledsByte = (ledsByte | BIT(led));
	else
		ledsByte = (ledsByte ^ BIT(led));
	returnValue = sendCommandtoKBC(SET_COMMAND);
	if(returnValue != 0)
		return returnValue;
	return sendCommandtoKBC(ledsByte);
}

int interruptNotification(int set){
	int r;
	message msg;
	int ipc_status;
	if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
		printf("driver_receive failed with: %d", r);
		return -2;
	}
	if (is_ipc_notify(ipc_status)) { /* received notification */
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE: /* hardware interrupt notification */
			if (msg.NOTIFY_ARG & set) {
				return 0;
			}
			break;
		default:
			break; /* no other notifications expected: do nothing */
		}
	}
	else { /* received a standard message, not a notification */
		/* no standard messages expected: do nothing */
	}
	return -1;
}
