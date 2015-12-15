#pragma once

#define BIT(n) (0x01 << n)
#define swap(type, i, j) {type t = i; i = j; j = t;}

#define OUT_BUF 	0x60
#define STAT_REG    0x64
#define OBF 		BIT(0)

#define PAR_ERR 	BIT(7)
#define TO_ERR 		BIT(6)


int fileExists(const char* filename);

int subscribeInt(int irqLine, int policy, int* hookId);

int unsubscribeInt(int* hookId);
