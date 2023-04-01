    CC     = gcc
    CFLAGS = -Wall -ggdb
    LFLAGS = -lm -ggdb 

	PROG = main

.PHONY: limpa faxina clean distclean purge all
all:	main.o bbrk.o malloc.so main

bbrk.o:	bbrk.s bbrk.h
	$(CC) -c $(CFLAGS) -shared -fPIC bbrk.s -o bbrk.o

main:	main.c
	$(CC) -o main $(LFLAGS) main.c

malloc.so:	malloc.c malloc.h bbrk.o
	$(CC) -O0 -W $(CFLAGS) -Wextra -shared -fPIC malloc.c -o malloc.so bbrk.o

clean:
	@rm -f *~ *.bak

purge:   clean
	@rm -f *.o core a.out
	@rm -f main

