AS =		gas
CC =		gcc
CFLAGS =	-ggdb -std=c99

CFILES = 	main.c
COBJECTS = 	$(CFILES:.c=.o)
BINARY = 	mysh

all: $(COBJECTS)
	$(CC) $(COBJECTS) -o $(BINARY)

.o: .c
	$(CC) $(CFLAGS) -c $<
