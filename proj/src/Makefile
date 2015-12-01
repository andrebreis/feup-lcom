# Makefile for the project driver (based on the labs).

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	main.c bitmap.c videoGraphics.c utilities.c vbe.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys 

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
