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
#define REG_B		11
#define UIP			BIT(7)
#define DATA_MODE	BIT(2)

/**
 * @struct Date
 * @brief keeps the Date info: year, month, day, hour and minute
 */
typedef struct{
	unsigned long year, month, day, hour, minute;
}Date;

/**
 * @brief reads the given register to the given value
 */
void readRegister(int reg, unsigned long *value);

/**
 * @brief reads the date from the RTC register and returns it
 */
Date getDate();

/**
 * @brief converts a Date struct to a Human-Friendly string
 */
char* dateToStr(Date date);

#endif /*__RTC_H_*/
