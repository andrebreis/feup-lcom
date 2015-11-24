#include <minix/drivers.h>

#include "keyboard.h"
#include "utilities.h"

static int kbHook = KB_IRQ;

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

int waitFor(unsigned char key) {
	message msg;
	int r, ipc_status;

	int returnValue;
	unsigned char firstByte = 0;

	int irq_set = subscribe_int(KB_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbHook);

	if (irq_set == -1)
		return -1;
	while (firstByte != key) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					returnValue = kbdReadKey(&firstByte);
					if (returnValue != 0)
						return returnValue;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}
	unsubscribe_int (&kbHook);
}
