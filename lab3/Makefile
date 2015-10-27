# Makefile for the lab3 driver (based on that of lab1).

COMPILER_TYPE= gnu

CC=gcc

PROG=	lab3
SRCS=	test3.c lab3.c kbd_interrupt_handler.S keyboard.c timer.c 

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
