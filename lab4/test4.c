//#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8042.h"
#include "i8254.h"

static int hook = MOUSE_IRQ;
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

int sendCommandtoKBC(unsigned long port, unsigned long cmd) {
	unsigned long stat;
	int returnValue;
	int i = 0;
	while (i < 10) {
		returnValue = sys_inb(STAT_REG, &stat);
		if (returnValue != 0)
			return returnValue;
		/* loop while 8042 input buffer is not empty */
		if ((stat & IBF) == 0) {
			returnValue = sys_outb(port, cmd); /*no args command*/
			if (returnValue != 0)
				return returnValue;
			else
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
}

int readFromKBC(char* readValue) {
	int returnValue;
	unsigned long stat, data;
	int i = 0;
	while (i < 3) {
		returnValue = sys_inb(STAT_REG, &stat);
		if (returnValue != 0) {
			//printf("error in 1st sysinb");
			return returnValue;
		}
		//printf("%d", stat);
		if (stat & OBF) {
			//printf("asd\n");
			returnValue = sys_inb(OUT_BUF, &data);
			if (returnValue != 0) {
				//printf("error in 2nd sysinb");
				return returnValue;
			}
			if ((stat & (PAR_ERR | TO_ERR)) == 0) {
				*readValue = data;
				return 0;
			} else {
				//printf("parr err or to err");
				return -1;
			}
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
	//printf("never found anything to read\n");
	return -1;
}

int writeToMouse(unsigned long cmd) {
	int returnValue, goodFeedback = 0;
	unsigned long cmdFeedback;

	while (!goodFeedback) {

		returnValue = sendCommandtoKBC(STAT_REG, WRITE_TO_MOUSE);
		if (returnValue != 0)
			return returnValue;

		returnValue = sendCommandtoKBC(OUT_BUF, cmd);
		if (returnValue != 0)
			return returnValue;

		returnValue = sys_inb(OUT_BUF, &cmdFeedback);
		if (returnValue != 0)
			return returnValue;

		if (cmdFeedback == ACK)
			goodFeedback = TRUE;
		else if (cmdFeedback == ERROR) {
			printf("Error sending command!\n");
			return -1;
		}
		//else if(cmd_Feedback == NACK)
		//	cmd = RESEND; //or should it be the same?
	}
	return 0;
}

int enableStreamMode() {
	return writeToMouse(SET_STREAM_MODE);
}

int disableStreamMode() {
	return writeToMouse(DIS_STREAM_MODE);
}

int enableSendingDataPackets() {
	int returnValue;

	sendCommandtoKBC(STAT_REG, 0xA8);

	returnValue = disableStreamMode();
	if (returnValue != 0)
		return returnValue;

	returnValue = writeToMouse(ENA_DATA_PACKS);
	if (returnValue != 0)
		return returnValue;

	return enableStreamMode();
}

int getPacket(char* packet) {
	int returnValue;
	returnValue = readFromKBC(packet);
	return returnValue;
}

void printPacket(char packet[3]) {
	char LB, MB, RB, XOV, YOV, xSign, ySign;
	int X, Y;

	printf("B1=0x%02X  B2=0x%02X  B3=0x%02X  ", (unsigned char) packet[0],
			(unsigned char) packet[1], (unsigned char) packet[2]);

	LB = (packet[0] & BIT(0)) != 0;
	RB = (packet[0] & BIT(1)) != 0;
	MB = (packet[0] & BIT(2)) != 0;
	XOV = (packet[0] & BIT(6)) != 0;
	YOV = (packet[0] & BIT(7)) != 0;
	xSign = (packet[0] & BIT(4)) != 0;
	ySign = (packet[0] & BIT(5)) != 0;

	X = packet[1];
	if (xSign) {
		X |= (-1 << 8);
	}
	Y = packet[2];
	if (ySign) {
		Y |= (-1 << 8);
	}

	printf("LB=%d  MB=%d  RB=%d  XOV=%d  YOV=%d  X=%d  Y=%d\n", LB, MB, RB, XOV,
			YOV, X, Y);
}

void printConfig(char status[3]) {

	if ((status[0] & BIT(6)) == 0) {
		printf("Stream Mode\n");
		if ((status[0] & BIT(5)) == 0)
			printf("Data reporting disabled\n");
		else
			printf("Data reporting enabled\n");
	} else
		printf("Remote Mode\n");

	if ((status[0] & BIT(4)) == 0)
		printf("Scaling is 1:1\n");
	else
		printf("Scaling is 2:1\n");

	if ((status[0] & BIT(2)) == 0)
		printf("Left button is currently released\n");
	else
		printf("Left button is currently pressed\n");

	if ((status[0] & BIT(1)) == 0)
		printf("Middle button is currently released\n");
	else
		printf("Middle button is currently pressed\n");

	if ((status[0] & BIT(0)) == 0)
		printf("Right button is currently released\n");
	else
		printf("Right button is currently pressed\n");

	printf("Resolution: ");
	switch (status[1]) {
	case 0:
		printf("1 count/mm\n");
		break;
	case 1:
		printf("2 count/mm\n");
		break;
	case 2:
		printf("4 count/mm\n");
		break;
	case 3:
		printf("8 count/mm\n");
		break;
	}

	printf("Sample rate: %d\n", status[2]);
}

int checkVerticalLine(char packet[3], int* xVar, int* yVar, short length,
		unsigned short tolerance) {
	printPacket(packet);
	printf("xvar: %d, yvar: %d, tolerance: %d \n", *xVar, *yVar, tolerance);
	if ((packet[0] & BIT(1)) == 0)
		*xVar = *yVar = 0;
	else {
		if ((packet[0] & BIT(5)) == 0) { // positive movement on y
			if (length > 0) {
				*yVar += packet[2];
			} else {
				*xVar = *yVar = 0; //if you are moving in the wrong direction, shouldnt add the horizontal movement to tolerance
			}
		} else {        //if (packet[0] & BIT(5)) == 0 -> negative movement on y
			if (length < 0)
				*yVar += (packet[2] | (-1 << 8));
			else
				*xVar = *yVar = 0; //if you are moving in the wrong direction, shouldnt add the horizontal movement to tolerance
		}
		if ((packet[0] & BIT(4)) == 0 && packet[1] != 0) {
			*xVar += packet[1];
		} else {
			if(packet[1] != 0)
			*xVar += (packet[1] | (-1 << 8));
		}
		if (abs(*xVar) >= tolerance) {
			*xVar = *yVar = 0;
			return 1;
		}
	}
	if (abs(*yVar) >= abs(length))
		return 0;
	else
		return 1;
}

int test_packet(unsigned short cnt) {
	message msg;
	int r, ipc_status;
	char packet[3];
	int returnValue, i = 0, mouseSet;
	mouseSet = subscribe_int(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
	if (mouseSet == -1) {
		return -1;
	}
	returnValue = enableSendingDataPackets();
	if (returnValue != 0) {
		return returnValue;
	}
	while (i < (cnt * 3)) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouseSet) { /* subscribed interrupt */
					returnValue = getPacket(&packet[i % 3]);
					if (returnValue == -1)
						continue;
					if (returnValue != 0) {
						return returnValue;
					}
					if ((i % 3 == 0) && (packet[i % 3] & BIT(3) == 0)) {
						continue;
					}
					if ((i % 3) == 2) {
						printPacket(packet);
					}
					i++;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else {
		}
	}
	disableStreamMode();
	unsubscribe_int(&hook);
	return 0;
}

int test_async(unsigned short idle_time) {
	message msg;
	int r, ipc_status;
	char packet[3];
	int returnValue, i = 0, mouseSet, timerSet, counter;
	mouseSet = subscribe_int(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
	if (mouseSet == -1) {
		return -1;
	}
	returnValue = enableSendingDataPackets();
	if (returnValue != 0) {
		return returnValue;
	}
	timerSet = subscribe_int(TIMER0_IRQ, IRQ_REENABLE, &timerHook);
	if (timerSet == -1) {
		return -1;
	}
	while (counter < (idle_time * 60)) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouseSet) { /* subscribed interrupt */
					returnValue = getPacket(&packet[i % 3]);
					if (returnValue == -1)
						continue;
					if (returnValue != 0) {
						return returnValue;
					}
					if ((i % 3 == 0) && (packet[i % 3] & BIT(3) == 0)) {
						continue;
					}
					if ((i % 3) == 2) {
						printPacket(packet);
						counter = 0;
					}
					i++;
				}
				if (msg.NOTIFY_ARG & timerSet)
					counter++;
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else {
		}
	}
	disableStreamMode();
	unsubscribe_int(&timerHook);
	unsubscribe_int(&hook);
	return 0;

}

int test_config(void) {
	message msg;
	int r, ipc_status;
	int mouseSet, returnValue, i;
	unsigned long stat;
	char status[3];

	mouseSet = subscribe_int(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
	if (mouseSet == -1)
		return -1;

	returnValue = disableStreamMode();
	if (returnValue != 0)
		return returnValue;

	returnValue = writeToMouse(STAT_REQ);
	if (returnValue != 0)
		return returnValue;

	while (i < 3) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouseSet) {
					returnValue = sys_inb(OUT_BUF, &stat);
					if (returnValue != OK)
						return returnValue;
					status[i] = stat;
					i++;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else {
		}
	}

	printConfig(status);
	unsubscribe_int(&hook);
	return 0;

}

int test_gesture(short length, unsigned short tolerance) {
	message msg;
	int r, ipc_status;
	int mouseSet, returnValue, i = 0;
	char packet[3];
	int xVar = 0;
	int yVar = 0;

	mouseSet = subscribe_int(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
	if (mouseSet == -1) {
		return -1;
	}
	returnValue = enableSendingDataPackets();
	if (returnValue != 0) {
		return returnValue;
	}

	while (1) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & mouseSet) { /* subscribed interrupt */
					returnValue = getPacket(&packet[i % 3]);
					if (returnValue == -1)
						continue;
					if (returnValue != 0) {
						return returnValue;
					}
					if ((i % 3 == 0) && (packet[i % 3] & BIT(3) == 0)) {
						continue;
					}
					if ((i % 3) == 2) {
						if (checkVerticalLine(packet, &xVar, &yVar, length,
								tolerance) == 0) {
							printf("Done!");
							disableStreamMode();
							unsubscribe_int(&hook);
							return 0;
						}
					}
					i++;
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else {
		}
	}

}
