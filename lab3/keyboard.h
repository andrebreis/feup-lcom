#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/syslib.h>

int kbc_subscribe_int(void);

int kbc_unsubscribe_int();

int sendCommandtoKBC(unsigned long cmd);

int kbdReadKey(unsigned char* resultKey);

int interruptNotification(message msg, int ipc_status, int kbc_set);

//int kbd_test_timed_scan(unsigned short n){
//	int irq_kbc, irq_timer, counter = 0, returnValue;
//	unsigned char firstByte;
//	int r;
//	message msg;
//	int ipc_status;
//	irq_timer = timer_subscribe_int();
//	irq_kbc = kbc_subscribe_int();
//
//	if(irq_timer == -1 || irq_kbc == -1)
//		return -1;
//	do{
//		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
//			printf("driver_receive failed with: %d", r);
//			return -2;
//		}
//
//		if (is_ipc_notify(ipc_status)){
//			switch (_ENDPOINT_P(msg.m_source)){
//			case HARDWARE:
//				if (msg.NOTIFY_ARG & irq_kbc){
//					returnValue = printCodes(&firstByte);
//					if(returnValue != 0)
//						return returnValue;
//				}
//				if (msg.NOTIFY_ARG & irq_timer)
//				{
//					counter++;
//				}
//				break;
//			default:
//				break;
//			}
//		}
//
//	}while(firstByte != ESC && counter < (n * 60));
//	kbc_unsubscribe_int();
//	timer_unsubscribe_int();
//}


#endif /*_LCOM_KEYBOARD_H_*/

