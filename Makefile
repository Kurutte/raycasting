CC = gcc
DEBUG = -g
CFLAGS = -Wall -std=c99 -pedantic -c $(DEBUG)
LFLAGS = -Wall -std=c99 -pedantic $(DEBUG)

all : raycast.o ppmrw.o v3math.o
	$(CC) $(LFLAGS) raycast.o ppmrw.o v3math.o -lm -o raycast

raycast.o : raycast.c raycast.h
	$(CC) $(CFLAGS) raycast.c

ppmrw.o : ppmrw.c ppmrw.h
	$(CC) $(CFLAGS) ppmrw.c

v3math.o : v3math.c v3math.h
	$(CC) $(CFLAGS) v3math.c

clean:
	\rm *.o raycast