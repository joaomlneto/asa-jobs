EXEC=jobs
CC=gcc
CFLAGS=-ansi -pedantic -Wall -O1 -lm -g
DEPS=defs.h testcase.h fifo.h defs.h
OBJ=jobs.o testcase.o fifo.o

default: $(EXEC)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

test: default
	./runtests.sh

clean:
	$(RM) -rf *.o $(EXEC) *.diff *.out $(EXEC).dSYM
