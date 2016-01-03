#include "RTC.h"

#include <minix/drivers.h>

void readRegister(int reg, unsigned long *value){
	sys_outb(RTC_ADDR, reg);
	sys_inb(RTC_DATA, value);
}

Date getDate(){
	Date returnDate;
	unsigned long regAByte;

	unsigned long returnValue;

	do{
		readRegister(REG_A, &regAByte);
	}while(regAByte & UIP);

	sys_outb(RTC_ADDR, MINUTE_REG);
	sys_inb(RTC_DATA, &returnValue);

	returnDate.minute = ((returnValue & 0xF0) >> 4) * 10 + (returnValue & 0x0F);

	sys_outb(RTC_ADDR, HOUR_REG);
	sys_inb(RTC_DATA, &returnValue);

	returnDate.hour = ((returnValue & 0xF0) >> 4) * 10 + (returnValue & 0x0F);

	sys_outb(RTC_ADDR, DAY_REG);
	sys_inb(RTC_DATA, &returnValue);

	((returnValue & 0xF0) >> 4) * 10 + (returnValue & 0x0F);
	returnDate.day = ((returnValue & 0xF0) >> 4) * 10 + (returnValue & 0x0F);

	sys_outb(RTC_ADDR, MONTH_REG);
	sys_inb(RTC_DATA, &returnValue);

	returnDate.month = ((returnValue & 0xF0) >> 4) * 10 + (returnValue & 0x0F);

	sys_outb(RTC_ADDR, YEAR_REG);
	sys_inb(RTC_DATA, &returnValue);

	returnDate.year = ((returnValue & 0xF0) >> 4) * 10 + (returnValue & 0x0F) + 2000;

	return returnDate;
}
