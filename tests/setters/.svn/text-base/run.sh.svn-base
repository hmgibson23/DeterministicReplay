#!/bin/bash


export DR_ROOT=/home/pt1110/workspace/DeterministicReplay
export TEST_DIR=tests/setters

cd $DR_ROOT/$TEST_DIR

export FOLLOW_AND_TERMINATE=yes
export KLEE_SCHED=$DR_ROOT/$TEST_DIR/test000040.schedule
export KLEE_VERBOSE_MODE=no
export LD_PRELOAD=$DR_ROOT/bin/libintercept.so

gdb --batch --command=gdbscript --args ./setters
#gdb ./setters

