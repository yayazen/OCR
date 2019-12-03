CC=gcc

CFLAGS  = -fsanitize=address -Wall -Wextra -std=c99 -O2 -lm $(INC) $(SRC)

CFLAGS	+= -rdynamic
CFLAGS  += `pkg-config gtk+-3.0 --cflags --libs`

LDFLAGS += `pkg-config gmodule-2.0 --libs`
# LDFLAG += `pkg-config gtkspell-2.0 --libs`


PRE = main.c
PRG = $(PRE:.c=)

DIR = ./net ./net/train ./proc
INC = $(foreach d, $(DIR), -I$d)
SRC = $(foreach d, $(DIR), $d/*.c)

.PHONY: clean

all: $(PRG)

clean:
	rm -f $(PRG)
