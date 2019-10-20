CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -O2

LDFLAGS =
LDLIBS= -lm

SRC = proc.c bmp.c img.c seg.c
TESTSRC = segTesting.c

OBJ = ${SRC:.c=.o}
TESTOBJ = ${TESTSRC:.c=.o}

DEP = ${OBJ:.o=.d}

all: proc

.PHONY: clean proc

proc: ${OBJ}

segTesting: CPPFLAGS = -D PRESENTATION
segTesting: ${TESTOBJ} ${OBJ} 

clean:
	${RM} ${OBJ} ${DEP} ${TESTOBJ} segTesting

-include ${DEP}

# END
