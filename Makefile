# Do not modify this makefile

CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -O2 -lm


DIR = ./net
INC = $(foreach d, $(DIR), -I$d)

SRC = XOR_simulation.c $(foreach d, $(DIR), $d/*.c)
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

MAIN = xor

.PHONY: clean

all:	
	$(CC) $(CFLAGS) $(INC) -o $(MAIN) $(SRC)


clean:
	rm -f  $(MAIN)

#EOF
