
gameoflife: gameoflife.o gol.o
	gcc -Wall -Wextra -pedantic -std=c11 gameoflife.o gol.o -o gameoflife
	
gameoflife.o: gameoflife.c
	gcc -Wall -Wextra -pedantic -std=c11 -c gameoflife.c
	
gol.o: gol.c gol.h
	gcc -Wall -Wextra -pedantic -std=c11 -c gol.c

clean: 
	rm *.o gameoflife