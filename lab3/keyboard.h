#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

int kbc_subscribe_int(void);

int kbc_unsubscribe_int();

int sendCommandtoKBC(unsigned long cmd);

int kbdReadKey(unsigned char* resultKey);


#endif /*_LCOM_KEYBOARD_H_*/
