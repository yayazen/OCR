# Do not modify this makefile

CC=gcc
CFLAGS= -Werror -Wall -Wextra -std=c99 -O2

all: main

.PHONY: main clean

main:
	+$(MAKE) -C proc
	+$(MAKE) -C nnet

clean:
	+$(MAKE) -C proc clean
	+$(MAKE) -C nnet clean

#EOF
