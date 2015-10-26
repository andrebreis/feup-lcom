#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "keyboard.h"
#include "i8042.h"
#include "timer.h"

int printCodes(unsigned char* firstByte){
	unsigned char secondByte;
	int returnValue = kbdReadKey(firstByte);
	if(returnValue != 0)
		return returnValue;
	if(*firstByte == 0xE0){
		returnValue = kbdReadKey(&secondByte);
		if(returnValue != 0)
			return returnValue;
		if((secondByte & BIT(7)) == 0)
			printf("MakeCode: 0x%02X%02X\n", *firstByte, secondByte);
		else
			printf("BreakCode: 0x%02X%02X\n", *firstByte, secondByte);
	}
	else
		if((*firstByte & BIT(7)) == 0)
			printf("MakeCode: 0x%02X\n", *firstByte);
		else
			printf("BreakCode: 0x%02X\n", *firstByte);
	return 0;
}

int kbd_test_scan(unsigned short ass) {

	int ipc_status;
	message msg;
	int returnValue;
	unsigned char firstByte;
	int kbc_set = kbc_subscribe_int();
	do{
		int driverReceive = interruptNotification(kbc_set);
		if (driverReceive == 0){
			returnValue = printCodes(&firstByte);
			if(returnValue != 0)
				return returnValue;
		}
		else if(driverReceive == -2)
			continue;
	}while(firstByte != ESC);
	kbc_unsubscribe_int();
	return 0;
}

int kbd_test_leds(unsigned short n, unsigned short *toggle){
	int i = 0, timerCounter = 0;
	int returnValue = setLedsToDefault(); //turns all leds off before executing
	if(returnValue == 0){
		int irq_set = timer_subscribe_int();
		if(irq_set == -1)
			return -1;
		while(timerCounter < n * 60){ //60hz -> 1 toggle / s
			if(interruptNotification(irq_set) == 0){
				timerCounter++;
				if(timerCounter % 60 == 0){
					returnValue = toggleLed(toggle[i]);
					if(returnValue == -7)
						printf("Invalid LED on element %d\n", i);
					else if(returnValue != 0)
						printf("Error sending command to KBC\n");
					else{
						printf("toggled, a second has passed\n");
						i++;
					}
				}
			}
			else
				continue;
		}
	}
	else
		return returnValue;
}

int kbd_test_timed_scan(unsigned short n){
	int irq_kbc, irq_timer, counter = 0, returnValue;
	unsigned char firstByte;
	int r;
	message msg;
	int ipc_status;
	irq_timer = timer_subscribe_int();
	irq_kbc = kbc_subscribe_int();

	if(irq_timer == -1 || irq_kbc == -1)
		return -1;
	do{
		if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
			printf("driver_receive failed with: %d", r);
			return -2;
		}

		if (is_ipc_notify(ipc_status)){
			switch (_ENDPOINT_P(msg.m_source)){
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_kbc){
					returnValue = printCodes(&firstByte);
					if(returnValue != 0)
						return returnValue;
				}
				if (msg.NOTIFY_ARG & irq_timer)
				{
					counter++;
				}
				break;
			default:
				break;
			}
		}

	}while(firstByte != ESC && counter < (n * 60));
	kbc_unsubscribe_int();
	timer_unsubscribe_int();
}

