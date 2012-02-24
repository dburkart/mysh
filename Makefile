# header.mak
AS =		gas
CC =		gcc
CFLAGS =	-ggdb -std=c99 -Wall

CFILES = 	main.c parse.c
COBJECTS = 	$(CFILES:.c=.o)
BINARY = 	mysh

all: $(COBJECTS)
	$(CC) $(COBJECTS) -o $(BINARY)

.o: .c
	$(CC) $(CFLAGS) -c $<
