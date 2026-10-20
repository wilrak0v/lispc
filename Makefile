all: main

main: main.c lexer.c
	gcc -o main main.c lexer.c -Wextra -Wall
