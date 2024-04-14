CFLAGS = -Wall
CC = gcc -g

INCLUDE = ./include
LIB = ./lib
OBJ = ./obj

all: csvreader 

csvreader: csvreader.o io.o stringItem.o filter.o free.o sort.o stat.o
	$(CC) -o csvreader $(OBJ)/*.o -lm

csvreader.o: csvreader.c $(INCLUDE)/*.h
	$(CC) -c $(CFLAGS) $< -I $(INCLUDE) -o $(OBJ)/$@

%.o: $(LIB)/%.c $(INCLUDE)/%.h
	$(CC) -c $(CFLAGS) $< -I $(INCLUDE) -o $(OBJ)/$@

clean:
	rm -f $(OBJ)/*.o 	

purge:
	rm -f $(OBJ)/*.o ./csvreader 	