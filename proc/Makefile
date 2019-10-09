CC=gcc
CFLAGS= -Werror -Wall -Wextra -std=c99 -O2

LDFLAGS =
LDLIBS= -lm

SRC = proc.c bmp.c img.c seg.c
TESTSRC = test.c

OBJ = ${SRC:.c=.o}
TESTOBJ = ${TESTSRC:.c=.o}

DEP = ${OBJ:.o=.d}

all: proc

.PHONY: clean proc

proc: ${OBJ}

#test: CPPFLAGS = -D PRESENTATION -D DEBUG
test: ${TESTOBJ} ${OBJ} 

clean:
	${RM} ${OBJ} ${DEP} ${TESTOBJ} test

-include ${DEP}

# END
