CC = gcc
CFLAGS = -Wall -Wextra -pthread

all: barrier rendezvous fork-join

barrier: barrier.c common_threads.h
	$(CC) $(CFLAGS) barrier.c -o barrier

rendezvous: rendezvous.c common_threads.h
	$(CC) $(CFLAGS) rendezvous.c -o rendezvous

fork-join: fork-join.c common_threads.h
	$(CC) $(CFLAGS) fork-join.c -o fork-join

clean:
	rm -f barrier rendezvous fork-join

