#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "keyboard.h"
#include "i8042.h"
#include "timer.h"

int kbd_test_scan(unsigned short ass) {
	unsigned char firstByte, secondByte;
	int returnValue;
	kbc_subscribe_int();
	do{
		returnValue = kbdReadKey(&firstByte);
		if(returnValue != 0)
			return returnValue;
		if(firstByte == 0xE0){
			returnValue = kbdReadKey(&secondByte);
			if(returnValue != 0)
				return returnValue;
			if((secondByte & BIT(7)) == 0)
				printf("MakeCode: 0x%02X%02X\n", firstByte, secondByte);
			else
				printf("BreakCode: 0x%02X%02X\n", firstByte, secondByte);
		}
		else
			if((firstByte & BIT(7)) == 0)
				printf("MakeCode: 0x%02X\n", firstByte);
			else
				printf("BreakCode: 0x%02X\n", firstByte);
	}while(firstByte != 0x81);
	kbc_unsubscribe_int();
	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *toggle){
	return 0;
}

int kbd_test_timed_scan(unsigned short n, unsigned short *toggle){
//	int ipc_status;
//	message msg;
//	int r, k = 0;
//
//	int irq_set = timer_subscribe_int();
//	kbc_subscribe_int();
//
//	if(irq_set == -1)
//		return irq_set;
//		while( k < n) { //60Hz, so 60 clock ticks per second
//			if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
//				printf("driver_receive failed with: %d", r);
//				continue;
//			}
//			if (is_ipc_notify(ipc_status)) { /* received notification */
//				switch (_ENDPOINT_P(msg.m_source)) {
//				case HARDWARE: /* hardware interrupt notification */
//					if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
//						timer_int_handler();
//						if(counter % 60 == 0){
//							toggleLed(toggle[k]);
//							k++;
//						}
//					}
//					break;
//				default:
//					break; /* no other notifications expected: do nothing */
//				}
//			}
//			else { /* received a standard message, not a notification */
//				/* no standard messages expected: do nothing */
//			}
//		}
//	kbc_unsubscribe_int();
//	return timer_unsubscribe_int();
	return 0;
}

