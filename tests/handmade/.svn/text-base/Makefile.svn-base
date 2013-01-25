

CC = gcc
CFLAGS = -Wall -g -c -pthread

all:
	make manymutex
	make setters
	make deadlock
	make philosophers
	make producers
	make barber
	make interceptrace

manymutex: manymutex.o 
	$(CC) -o manymutex manymutex.o -pthread
manymutex.o: manymutex.c
	$(CC) $(CFLAGS) manymutex.c
setters: setters.o
	$(CC) -o setters setters.o -pthread
setters.o: 
	$(CC) $(CFLAGS) setters.c
deadlock: deadlock.o
	$(CC) -o deadlock deadlock.o -pthread
deadlock.o:
	$(CC) $(CFLAGS) deadlock.c
philosophers: dp.o
	$(CC) -o philosophers dp.o -pthread
dp.o:
	$(CC) $(CFLAGS) dp.c
producers: producers.o
	$(CC) -o producers producers.o -pthread
producers.o:
	$(CC) $(CFLAGS) producers.c
barber: barber.o
	$(CC) -o barber barber.o -pthread
barber.o:
	$(CC) $(CFLAGS) barber.c
interceptrace: interceptrace.o
	$(CC) -o interceptrace interceptrace.o -pthread
interceptrace.o: 
	$(CC) $(CFLAGS) interceptrace.c



clean:
	rm -f manymutex.o manymutex setters.o setters deadlock.o deadlock producers producers.o bounded bounded.o barber barber.o dp.o philosophers interceptrace interceptrace.o