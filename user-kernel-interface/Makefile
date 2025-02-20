CC = gcc
CFLAGS = -g -Wall -I/usr/include/libnl3
LDFLAGS = -lnl-3 -lnl-genl-3

taskstats: taskstats.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

segfault:
	gcc -g ./segfault.c -rdynamic
