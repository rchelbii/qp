CC = gcc
FLAGS = 
TARGET = build/db
SOURCE = main.c

all:
	$(CC) $(FLAGS) $(SOURCE) $(EXTRALIB) -o $(TARGET) 

remove:
	rm -f *.o
