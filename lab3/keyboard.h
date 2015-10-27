#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/syslib.h>

int kbc_subscribe_int(void);

int kbc_unsubscribe_int();

int sendCommandtoKBC(unsigned long cmd);

int kbdReadKey(unsigned char* resultKey);

int interruptNotification(message msg, int ipc_status, int kbc_set);

#endif /*_LCOM_KEYBOARD_H_*/

