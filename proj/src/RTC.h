#ifndef __RTC_H_
#define __RTC_H_

#include "utilities.h"

#define RTC_IRQ		8
#define RTC_ADDR 	0x70
#define RTC_DATA 	0x71
#define MINUTE_REG	2
#define HOUR_REG	4
#define DAY_REG		7
#define MONTH_REG	8
#define YEAR_REG	9
#define REG_A		10
#define UIP			BIT(7)

typedef struct{
	unsigned long year, month, day, hour, minute;
}Date;

void readRegister(int reg, unsigned long *value);

Date getDate();

#endif /*__RTC_H_*/
