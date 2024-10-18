CC=gcc
CFLAGS=-I.

all : parallel_min_max zombie process_memory parallel_sum

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

zombie : 
	$(CC) -o zombie zombie.c $(CFLAGS)

process_memory : 
	$(CC) -o process_memory process_memory.c $(CFLAGS)

parallel_sum : utils.o
	$(CC) -o parallel_sum utils.o parallel_sum.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o parallel_min_max zombie process_memory parallel_sum
