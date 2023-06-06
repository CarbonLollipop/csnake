CC=gcc
CFLAGS=-Wall -Wextra

csnake: main.o
	gcc -o csnake main.o -lncurses -Wl,-rpath=/usr/local/lib -L/usr/local/lib

main.o: main.c
	gcc -c -o main.o main.c

install:
	sudo cp csnake /usr/local/bin/

clean:
	rm -f csnake main.o

