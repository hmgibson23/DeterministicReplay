

all: setters

setters: setters.c
	gcc -O0 -g -o setters setters.c -funwind-tables -pthread -rdynamic 

test:
	./run.sh

clean:
	rm -f setters

