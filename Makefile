gameoflife: gameoflife.o libgol.so
	gcc -Wall -Wextra -pedantic -std=c11 gameoflife.o gol.o -o gameoflife
	
gameoflife.o: gameoflife.c
	gcc -Wall -Wextra -pedantic -std=c11 -c gameoflife.c

libgol.so: gol.o
	gcc -Wall -Wextra -pedantic -std=c11 -shared -o libgol.so gol.o

gol.o: gol.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -fpic -c gol.c

clean: 
	rm *.o *.so gameoflife