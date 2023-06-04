CC = gcc
FLAGS = 
TARGET = build/db
SOURCE = main.c

all:
	mkdir -p build/
	$(CC) $(FLAGS) $(SOURCE) $(EXTRALIB) -o $(TARGET) 

clear:
	rm -f *.o
