#include <minix/syslib.h>
#include <minix/drivers.h>

int timer_set_square(unsigned long timer, unsigned long freq) {

	return 1;
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

	sys_outb(TIMER_CTRL, readBack);
	sys_inb(TIMER_0+timer, st);
	return 0;
}

int timer_display_conf(unsigned char conf) {
	//TODO lab2
	return 1;
}

int timer_test_square(unsigned long freq) {
	//TODO lab2
	return 1;
}

int timer_test_int(unsigned long time) {
	//TODO lab3
	return 1;
}

int timer_test_config(unsigned long timer) {
	//TODO lab2
	return 1;
}
