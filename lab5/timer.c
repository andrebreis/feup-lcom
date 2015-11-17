#include <minix/drivers.h>

#include "timer.h"

static int timerHook = TIMER0_IRQ;

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

int timer_test_int(unsigned long time) {
	int ipc_status;
	message msg;
	int r;
	int counter = 0;

	int irq_set = subscribe_int(TIMER0_IRQ, IRQ_REENABLE, &timerHook);

	if(irq_set == -1)
		return irq_set;

	while( counter < time * 60) { //60Hz, so 60 clock ticks per second
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					counter ++;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		}
		else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	return unsubscribe_int(&timerHook);
}
