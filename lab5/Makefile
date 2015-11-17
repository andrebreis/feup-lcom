# Makefile for the lab5 driver (based on the other labs).

COMPILER_TYPE= gnu

CC=gcc

PROG=	lab5
SRCS=	test5.c lab5.c video_gr.c vbe.c timer.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -ldriver -lsys -llm

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
