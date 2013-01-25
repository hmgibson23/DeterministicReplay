wrap: wrappertest.o
	gcc -pthread wrappertest.o -o wrap
wrapper.o:
	gcc -ggdb -Wall -Wextra -pthread -c wrappertest.c
clean:
	rm -f wrappertest.o wrap