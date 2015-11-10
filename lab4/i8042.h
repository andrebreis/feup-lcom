#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define DELAY_US    20000

#define OUT_BUF 	0x60
#define STAT_REG    0x64
#define KBC_CMD_REG 0x64
#define BIT(n) (	0x01<<(n))
#define KB_IRQ 		1
#define PAR_ERR 	BIT(7)
#define TO_ERR 		BIT(6)
#define OBF 		BIT(0)
#define IBF 		BIT(1)
#define SET_COMMAND 0xED
#define ESC			0x81

#define MOUSE_IRQ		12
#define WRITE_TO_MOUSE 	0xD4
#define ACK				0xFA
#define NACK			0xFE
#define ERROR 			0xFC
#define RESEND 			0xFE
#define STAT_REQ		0xE9

#define TRUE	1







#endif /* _LCOM_I8042_H_ */
