CC = gcc
FLAGS = -Wall -gcc

all: thead_q 

prime.o: prime.c
	$(CC) $(FLAGS) -c prime.c