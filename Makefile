
gameoflife: gameoflife.o libgol.so
	gcc -Wall -Wextra -pedantic -std=c11 gameoflife.o -L. -lgol -o gameoflife

gameoflife.o: gameoflife.c
	gcc -Wall -Wextra -pedantic -std=c11 -c gameoflife.c -o gameoflife.o

libgol.so: gol.o
	gcc -Wall -Wextra -pedantic -std=c11 -shared gol.o -o libgol.so -L.

gol.o: gol.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -fpic -c gol.c -o gol.o

clean:
	rm *.o *.so gameoflife
