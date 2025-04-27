all: main.c
	./script.sh
	gcc -Wall -o dashboard main.c -lpthread

dashboard:
	gcc -Wall -o dashboard main.c -lpthread

content:
	./script.sh

run:
	./dashboard

clean:
	rm dashboard
	rm news.txt
	rm sports.txt
	rm crypto.txt
	rm stocks.txt
