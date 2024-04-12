CFLAGS = -Wall
CC = gcc -g

all: csvreader 

csvreader: csvreader.o io.o stringItem.o floatItem.o
	$(CC) -o csvreader *.o -lm

io.o: io.c io.h
	$(CC) -c $(CFLAGS) $< -o ./$@

csvreader.o: csvreader.c io.h
	$(CC) -c $(CFLAGS) $< ./$@

stringItem.o : stringItem.c stringItem.h
	$(CC) -c $(CFLAGS) $< -o ./$@

floatItem.o : floatItem.c floatItem.h
	$(CC) -c $(CFLAGS) $< -o ./$@

clean:
	rm -f ./*.o 

purge:
	rm -f ./*.o csvreader