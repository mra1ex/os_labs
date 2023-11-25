CC=gcc
CFLAGS=-I.

all : parallel_min_max zombie process_memory parallel_sum

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

parallel_sum : utils.o sum.o utils.h sum.o
	$(CC) -o parallel_sum utils.o sum.o parallel_sum.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.0 : find_min_max.h utils.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

process_memory : process_memory.c
	$(CC) -o process_memory process_memory.c

sum.o : sum.h
	$(CC) -o sum.o -c sum.c $(CFLAGS)

zombie : zombie.c
	$(CC) -o zombie -c zombie.c $(CFLAGS)

clean :
	rm -f utils.o sum.o parallel_min_max parallel_sum process_memory
