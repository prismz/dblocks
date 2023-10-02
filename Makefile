all:
	cc *.c elements/*.c -lm -lX11 -g -Wall -Wextra -pedantic -o dblocks

run:all
	./dblocks

install:all
	cp dblocks /usr/local/bin/
	cp dblocks-tidal.sh /usr/local/bin/
