#Make file for Deterministic Replay function intercepting

#MACROS
CC = gcc
CXX = g++
LINK = gcc  -o 
COMPILEFLAGS = -ggdb -Wall -shared -ldl -fPIC -pthread 
LINKINGFLAGS = -pthread -lstdc++ -ldl -shared -rdynamic
OUT = ../../bin/

intercept.so: intercept.o thread_map.o read_file.o build_sched.o PrintTrace.o
	$(LINK) $(OUT)libintercept.so intercept.o read_file.o thread_map.o PrintTrace.o build_sched.o $(LINKINGFLAGS)
	rm -f thread_map.o intercept.o
intercept.o:
	$(CC) $(COMPILEFLAGS) -c intercept.c
thread_map.o:
	$(CXX) $(COMPILEFLAGS) -c thread_map.cpp
read_file.o:
	$(CXX) $(COMPILEFLAGS) -c read_file.cpp
build_sched.o:
	$(CXX) $(COMPILEFLAGS) -c build_sched.cpp
PrintTrace.o:
	$(CXX) $(COMPILEFLAGS) -c PrintTrace.cpp

clean:
	rm -f $(OUT)libintercept.so thread_map.o intercept.o read_file.o build_sched.o PrintTrace.o