#include <minix/drivers.h>

#include "Timer.h"
#include "utilities.h"

static int hook = TIMER0_IRQ;

int subscribeTimerInt(){
	return subscribeInt(TIMER0_IRQ, IRQ_REENABLE, &hook);
}

int unsubscribeTimerInt(){
	return unsubscribeInt(&hook);
}
