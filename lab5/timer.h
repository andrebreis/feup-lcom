#ifndef __TIMER_H
#define __TIMER_H

#define TIMER0_IRQ 0
#define BIT(n) (	0x01<<(n))

int subscribe_int(int irqLine, int policy, int* hookId);

int unsubscribe_int(int* hookId);

int timer_test_int(unsigned long time);

#endif /* __TIMER_H */
