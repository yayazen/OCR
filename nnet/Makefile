# Makefile
CC=gcc
CFLAGS=-Wall -Werror -Wextra -std=c99 -O2

SRC = nnet.c
TESTSRC = test.c 

OBJ = ${SRC:.c=.o}
TESTOBJ = ${TESTSRC:.c=.o}

 
DEP = ${OBJ:.o=.d}

all: nnet

.PHONY: nnet clean

nnet: ${OBJ}

test: ${OBJ} ${TESTOBJ}

clean:
	${RM} ${OBJ} ${DEP} ${TESTOBJ} test

-include ${DEP}

# END
