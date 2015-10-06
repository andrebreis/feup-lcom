#include <minix/syslib.h>
#include <minix/drivers.h>

int timer_set_square(unsigned long timer, unsigned long freq) {
	if(timer > 2 || timer < 0)
		return -1;

	int div = TIMER_FREQ/freq;
	char lsb = (char) div;
	char msb = (char) div>>8;
	char timerInfo;

	if(timer == 0)
		timerInfo = TIMER_SEL0;
	else
		timerInfo = BIT(timer+5);

	char setDiv = timerInfo | TIMER_LSB_MSB | TIMER_SQR_WAVE | TIMER_BIN;
	int returnValue = sys_outb(TIMER_CTRL, setDiv);
	if(returnValue != 0){
		printf("Error in sys_outb");
		return returnValue;
	}

	returnValue = sys_outb(TIMER_0+timer, lsb);

	if(returnValue != 0){
			printf("Error in sys_outb");
			return returnValue;
		}

	returnValue = sys_outb(TIMER_0+timer, msb);

	if(returnValue != 0){
			printf("Error in sys_outb");
			return returnValue;
		}

	return 0;
}

int timer_subscribe_int(void ) {

	return 1;
}

int timer_unsubscribe_int() {

	return 1;
}

void timer_int_handler() {

}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	if(timer > 2 || timer < 0)
		return -1;

	char readBack = TIMER_RB_CMD|TIMER_RB_SEL(timer)|TIMER_RB_COUNT_;

	int returnValue = sys_outb(TIMER_CTRL, readBack);
	if(returnValue != 0){
		printf("Error in sys_outb");
		return returnValue;
	}
	returnValue = sys_inb(TIMER_0+timer, st);
	if(returnValue != 0){
		printf("Error in sys_inb");
		return returnValue;
	}
	return 0;
}

int timer_display_conf(unsigned char conf) {
	int output = conf>>7 & BIT(0); //0000000BIT(7)
	int nullCount = conf>>6 & BIT(0); //0000000BIT(6)
	int accessType = conf>>4 & (BIT(1)|BIT(0)); //000000BIT(5)BIT(4)
	int operatingMode = conf>>1 & (BIT(2)|BIT(1)|BIT(0)); //00000BIT(3)BIT(2)BIT(1)
	int countingMode = conf & BIT(0);
	if(accessType == 0){
		printf("Error in Type of Access bits");
		return -1;
	}
	printf("Output: ");
	if(output)
		printf("True\n");
	else
		printf("False\n");
	printf("Null Count: ");
	if(nullCount)
		printf("True\n");
	else
		printf("False\n");
	printf("Type of Access: ")
	switch(accessType){
		case 1:
			printf("LSB\n");
			break;
		case 2:
			printf("MSB\n");
			break;
		case 3:
			printf("LSB followed by MSB\n");
			break;
		default:
			return -1;
	}
	printf("Operating Mode: ");
	switch(operatingMode){
		case 0:
			printf("Interrupt on Terminal Count\n");
			break;
		case 1:
			printf("Hardware Retriggerable One-Shot\n");
			break;
		case 2:
			printf("Rate Generator\n");
			break;
		case 3:
			printf("Square Wave Mode\n");
			break;
		case 4:
			printf("Software Triggered Strobe\n");
			break;
		case 5:
			printf("Hardware Triggered Strobe (Retriggerable)\n");
			break;
		default:
			return -2;
	}
	printf("Counting Mode: ");
	switch(countingMode){
		case 0:
			printf("Binary (16 bits)\n");
			break;
		case 1:
			printf("BCD (4 decades)\n");
			break;
		default:
			return -3;
	}
	return 0;
}

int timer_test_square(unsigned long freq) {
	return timer_set_square(0, freq);
}

int timer_test_int(unsigned long time) {
	//TODO lab3
	return 1;
}

int timer_test_config(unsigned long timer) {
	char *st;
	int returnValue = timer_get_conf(timer, st);
	if(returnValue != 0){
		printf("Error in timer_get_conf");
		return returnValue;
	}
	returnValue = timer_display_conf(st);
	if(returnValue != 0){
		printf("Error in timer_display_conf");
		return returnValue;
	}
	return 0;
}
