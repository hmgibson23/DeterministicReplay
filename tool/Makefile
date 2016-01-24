#Makefile for deterministic replay tool
CC = gcc
CXX = g++
CFLAGS = -g -Wall -shared -ldl -fPIC -pthread -I/usr/include/malloc
LFLAGS = -pthread -lstdc++ -ldl -shared
BIN = ../bin/
LOC = ../src/intercept/

multiple:
	$(CC) $(CFLAGS) multiple.c -o multiple

condtest:
	$(CC) $(CFLAGS) condtest.c -o condtest
