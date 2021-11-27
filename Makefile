CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -Werror

setcal: setcal.c
	$(CC) $(CFLAGS) setcal.c -o setcal