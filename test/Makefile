CC = gcc
CFLAGS = -Wall -I$(INCDIR)

BASENAME = dstructs
HEADER = test.h

INCDIR = ../include/
LIBDIR = ../lib/
BINDIR = ../bin/

SRCS = $(wildcard *.c)
TESTS = $(patsubst %.c, $(BINDIR)%, $(SRCS))

.PHONY: run

run: $(TESTS)
	for test in $(TESTS); do \
		./$$test; \
	done

$(BINDIR)%: %.c $(HEADER)
	@$(CC) $(CFLAGS) $< -L$(LIBDIR) -l$(BASENAME) -o $@