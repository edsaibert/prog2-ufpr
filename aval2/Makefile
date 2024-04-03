CFLAGS = -Wall
CC = gcc -g

all: csvreader 

csvreader: csvreader.o io.o 
	$(CC) -o csvreader *.o -lm

io.o: io.c io.h
	$(CC) -c $(CFLAGS) $< -o ./$@

csvreader.o: csvreader.c io.h
	$(CC) -c $(CFLAGS) $< ./$@

clean:
	rm -f ./*.o 

flush:
	rm -f ./*.o csvreader