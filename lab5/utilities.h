#ifndef __UTILITIES_H
#define __UTILITIES_H

#define BIT(n) (	0x01<<(n))

int subscribe_int(int irqLine, int policy, int* hookId);

int unsubscribe_int(int* hookId);

#endif /* __UTILITIES_H */
