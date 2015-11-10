//#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8042.h"

//static int hook = KB_IRQ;

int subscribe_int(int irqLine, int policy, int* hookId) {
	int bitmask = BIT(*hookId);
	if (sys_irqsetpolicy(irqLine, policy, hookId) == OK)
		if (sys_irqenable(hookId) == OK)
			return bitmask;
	return -1;
}

int unsubscribe_int(int* hookId) {
	if (sys_irqdisable(hookId) == OK)
		if (sys_irqrmpolicy(hookId) == OK)
			return 0;
	return 1;
}

int interruptNotification(message msg, int ipc_status, int set){
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

int test_packet(unsigned short cnt){
    //get packet
	//
	/* To be completed ... */
}	
	
int test_async(unsigned short idle_time) {
    /* To be completed ... */
}
	
int test_config(void) {
    /* To be completed ... */
}	
	
int test_gesture(short length, unsigned short tolerance) {
    /* To be completed ... */
}
