#include "utilities.h"

#include <stdio.h>
#include <minix/drivers.h>

int fileExists(const char* filename){
	FILE* file = fopen(filename, "r");

	if(file){
		fclose(file);
		return 1;
	}

	return 0;
}


int subscribeInt(int irqLine, int policy, int* hookId) {
	int bitmask = BIT(*hookId);
	if (sys_irqsetpolicy(irqLine, policy, hookId) == OK)
		if (sys_irqenable(hookId) == OK)
			return bitmask;
	return -1;
}

int unsubscribeInt(int* hookId) {
	if (sys_irqdisable(hookId) == OK)
		if (sys_irqrmpolicy(hookId) == OK)
			return 0;
	return 1;
}
