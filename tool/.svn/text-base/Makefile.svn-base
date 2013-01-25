#Makefile for deterministic replay tool
CC = gcc
CXX = g++
CFLAGS = -g -Wall -shared -ldl -fPIC -pthread -I/usr/include/malloc 
LFLAGS = -pthread -lstdc++ -ldl -shared
BIN = ../bin/
LOC = ../src/intercept/

all: $(BIN)libintercept.so

$(BIN)libintercept.so: intercept.o thread_map.o read_file.o build_sched.o PrintTrace.o
	$(CC) -o $(BIN)libintercept.so intercept.o thread_map.o read_file.o build_sched.o PrintTrace.o $(LFLAGS)
intercept.o: $(LOC)intercept.c
	$(CC) $(CFLAGS) -c $(LOC)intercept.c
thread_map.o: $(LOC)thread_map.cpp
	$(CXX) $(CFLAGS) -c $(LOC)thread_map.cpp
read_file.o: $(LOC)read_file.cpp
	$(CXX) $(CFLAGS) -c $(LOC)read_file.cpp
build_sched.o: $(LOC)build_sched.cpp
	$(CXX) $(CFLAGS) -c $(LOC)build_sched.cpp
PrintTrace.o: $(LOC)PrintTrace.cpp
	$(CXX) $(CFLAGS) -c $(LOC)PrintTrace.cpp
clean:
	rm -f $(BIN)libintercept.so thread_map.o intercept.o read_file.o build_sched.o PrintTrace.o
