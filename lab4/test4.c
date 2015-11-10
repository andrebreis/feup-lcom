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

int sendCommandtoKBC(unsigned long port, unsigned long cmd){
	unsigned long stat;
	int returnValue;
	int i = 0;
	while( i < 10 ) {
		returnValue = sys_inb(STAT_REG, &stat);
		if(returnValue != 0)
			return returnValue;
		/* loop while 8042 input buffer is not empty */
		if( (stat & IBF) == 0 ) {
			returnValue = sys_outb(port, cmd); /*no args command*/
			if(returnValue != 0)
				return returnValue;
			else
				return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
}

int readFromKBC(char* readValue){
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
				*readValue = data;
				return 0;
			}
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
	return -1;
}

int writeToMouse(unsigned long cmd){
	int returnValue, goodFeedback = 0;
	unsigned long cmdFeedback;

	while(!goodFeedback){

	returnValue = sendCommandtoKBC(STAT_REG, WRITE_TO_MOUSE);
	if(returnValue != 0)
		return returnValue;

	returnValue = sendCommandtoKBC(OUT_BUF, cmd);
	if(returnValue != 0)
		return returnValue;

	returnValue = sys_inb(OUT_BUF, &cmdFeedback);
	if(returnValue != 0)
		return returnValue;

	if(cmdFeedback == ACK)
		goodFeedback = TRUE;
	else if(cmdFeedback == ERROR){
		printf("Error sending command!\n");
		return -1;
	}
	//else if(cmd_Feedback == NACK)
	//	cmd = RESEND; //or should it be the same?
	}
	return 0;
}

int enableStreamMode(){
	return writeToMouse(SET_STREAM_MODE);
}

int disableStreamMode(){
	return writeToMouse(DIS_STREAM_MODE);
}

int enableSendingDataPackets(){
	int returnValue;

	returnValue = disableStreamMode();
	if(returnValue != 0)
		return returnValue;

	returnValue = writeToMouse(ENA_DATA_PACKS);
	if(returnValue != 0)
		return returnValue;

	return enableStreamMode();
}

int getPacket(char* packet){
	int returnValue;

	returnValue = readFromKBC(packet);
	return returnValue;
}

void printPacket(char packet[]){
	char LB, MB, RB, XOV, YOV, xSign, ySign;
	int X = 0, Y = 0;
	printf("B1=%02X  B2=%02X  B3=%02X  ", packet[0], packet[1], packet[2]);
	LB = packet[0] & BIT(0);
	RB = packet[0] & BIT(1);
	MB = packet[0] & BIT(2);
	XOV = packet[0] & BIT(6);
	YOV = packet[0] & BIT(7);
	xSign = packet[0] & BIT(4);
	ySign = packet[0] & BIT(5);
	if(xSign)
		X = (~packet[1] + 1);
	else
		X = packet[1];
	if(ySign)
			Y = (~packet[2] + 1);
		else
			Y = packet[2];
	printf("LB=%02X  MB=%02X  RB=%02X  XOV=%d  YOV=%d  X=%d  Y=%d\n", LB, MB, RB, XOV, YOV, X, Y);
}

/*int test_packet(unsigned short cnt){
	message msg;
	int r, ipc_status;
    char packet[3];
    int returnValue, i = 0, mouseSet;

    mouseSet = subscribe_int(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, )

    while(i < (cnt*3)){
    	if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
    					printf("driver_receive failed with: %d", r);
    					continue;
    				}
    				if (interruptNotification(msg, ipc_status, kbc_set) == 0){
    					returnValue = getPacket(packet[i % 3]);
    					if(returnValue != 0)
    						return returnValue;
    				}

    if(returnValue != 0)
    	return -1;
    if(i % 3 == 0 && (packet[i % 3] & BIT(3) == 0)
    	continue;

    }
	//get packet
	//
	/* To be completed ... */
//}
	
int test_async(unsigned short idle_time) {
    /* To be completed ... */
}
	
int test_config(void) {
    /* To be completed ... */
}	
	
int test_gesture(short length, unsigned short tolerance) {
    /* To be completed ... */
}
