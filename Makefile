all: main

CC = clang

main:
	$(CC) main.c fonctions.c -o main
