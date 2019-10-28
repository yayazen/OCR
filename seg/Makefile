# Simple makefile, do not modify

CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -O2 -lm $(SRC) $(INC) -D PRESENTATION


DIR = ./proc
INC = $(foreach d, $(DIR), -I$d)

PRE = segTesting.c

PRG = ${PRE:.c=}
SRC = $(foreach d, $(DIR), $d/*.c)
OBJ = ${SRC:.c=.o}


.PHONY: clean

all: $(PRG)

clean:
	rm -f  $(PRG) *.bmp

#EOF
