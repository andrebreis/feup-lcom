//static int hook = KB_IRQ;

int subscribe_int(int irqLine, int policy, int* hookId) {
	int bitmask = BIT(hookId);
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

int test_packet(unsigned short cnt){
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
