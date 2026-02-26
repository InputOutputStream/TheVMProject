flags=-O2 -Wall -Wextra -std=c2x
ldflags=-lbu

.PHONY: all clean

all: clean THOTH_VM

THOTH_VM: THOTH_VM.o
	cc $(flags) $^ -o $@ $(ldflags)

THOTH_VM.o: THOTH_VM.c THOTH_VM.h
	cc $(flags) -c $<

clean:
	rm -f *.o THOTH_VM