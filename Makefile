all: main

main: main.c lexer.c parser.c
	gcc -o main main.c lexer.c parser.c -Wextra -Wall
