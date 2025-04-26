all: main.c
	gcc -Wall -o dashboard main.c -lpthread

run:
	./dashboard

clean:
	rm dashboard
