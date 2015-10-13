CFLAGS = -g --std=c99

*.o: *.c
	cc -c ${CFLAGS} $^

test: r6parity.c galoisfield.o parity.o
	cc ${CFLAGS} -o r6parity $^
	./r6parity
clean:
	rm -f *.o r6parity
