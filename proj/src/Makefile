# Makefile for the project driver (based on the labs).

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	main.c bitmap.c videoGraphics.c utilities.c vbe.c Mouse.c Timer.c Duck.c Keyboard.c AnimSprite.c DuckHunt.c Scores.c RTC.c

CCFLAGS= -Wall -O3

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+= -llm -ldriver -lsys 

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
