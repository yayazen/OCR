# Do not modify this makefile

CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -O2

SRC = proc/proc.c proc/bmp.c
OBJ = ${SRC:.c=.o}

main: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}
#EOF
