#ifndef __TIMER_H
#define __TIMER_H

#define TIMER0_IRQ 0

/**
 * @brief subscribes timer interrupts
 */
int subscribeTimerInt();

/**
 * @brief unsubscribes timer interrupts
 */
int unsubscribeTimerInt();

#endif /* __TIMER_H */
