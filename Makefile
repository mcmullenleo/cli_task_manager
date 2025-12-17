CC = gcc
CFLAGS = -Wall -g

todo: main.c
		$(CC) $(CFLAGS) -o todo main.c

clean:
		rm -f todo