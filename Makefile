all:
	gcc -Wall -o dashboard main.c -lpthread

run:
	./dashboard
