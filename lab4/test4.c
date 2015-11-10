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

	returnValue = sys_inb(OUT_BUF, &cmd_Feedback);
	if(returnValue != 0)
		return returnValue;

	if(cmd_Feedback == ACK)
		goodFeedback = TRUE;
	else if(cmd_Feedback == ERROR){
		printf("Error sending command!\n");
		return -1;
	}
	//else if(cmd_Feedback == NACK)
	//	cmd = RESEND; //or should it be the same?
	}
	return 0;
}

/*char getPacket(){

}*/

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
