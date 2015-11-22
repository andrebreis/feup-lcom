#include <minix/drivers.h>

#include "timer.h"
#include "keyboard.h"
#include "utilities.h"

static int kbHook = KB_IRQ;

void *test_init(unsigned short mode, unsigned short delay) {

	vg_init(mode);

	timer_test_int(delay);

	if (vg_exit() == 1)

		return 1;

}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {
	message msg;
	int r, ipc_status;

	int returnValue;
	unsigned char firstByte = 0;

	int irq_set = subscribe_int(KB_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbHook);

	if(irq_set == -1)
		return -1;

	vg_init(0x105);

	vg_draw_square(x, y, size, color);

	while(firstByte != ESC){
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					returnValue = kbdReadKey(&firstByte);
					if(returnValue != 0)
						return returnValue;
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

	vg_exit();

	unsubscribe_int(&kbHook);
}

int test_line(unsigned short xi, unsigned short yi,

		unsigned short xf, unsigned short yf, unsigned long color) {

	message msg;
	int r, ipc_status;

	int returnValue;
	unsigned char firstByte = 0;

	int irq_set = subscribe_int(KB_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbHook);

	if(irq_set == -1)
		return -1;

	vg_init(0x105);

	vg_draw_line(xi, yi, xf, yf, color);

	while(firstByte != ESC){
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					returnValue = kbdReadKey(&firstByte);
					if(returnValue != 0)
						return returnValue;
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

	unsubscribe_int(&kbHook);

	vg_exit();

}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	/* To be completed */

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],

		unsigned short hor, short delta, unsigned short time) {

	/* To be completed */

}

int test_controller() {

	/* To be completed */

}

