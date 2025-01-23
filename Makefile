all:
	cc *.c elements/*.c -lm -lX11 -g -Wall -Wextra -pedantic -o dblocks

run:all
	./dblocks

install:all
	cp -f dblocks /usr/local/bin/
	cp elements/dblocks-spotify.sh /usr/local/bin/
	#cp elements/dblocks-tidal.sh /usr/local/bin/
