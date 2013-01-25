#Newer version of the tool
import os
import subprocess
import sys


def parseExecutionMode(N):
    if N == "-e":
        os.environ['FOLLOW_AND_RESUME'] = "YES"
        print "Running in FOLLOW_TO_END mode ....\n"
        return N
    elif N == "-t":
        os.environ['FOLLOW_AND_TERMINATE'] = "yes"
        print "Running in FOLLOW_AND_TERMINATE mode ....\n"
        return N
    elif N == "-r":
        os.environ['RECORD_MODE'] = "yes"
        print "Running in RECORD_MODE ...\n"
        return N
    elif N == "-n":
        print "Running is default execution mode ...\n"
        return N
    elif N == "-m":
        print "Running in monitor mode ...\n"
        os.environ['MONITOR_MODE'] = "yes"
        return N
    else:
        print "No such mode of execution exists"
        print "Defaulting to monitor mode....."
        os.environ['MONITOR_MODE'] = "yes"
        return N

record = False
commarg1 = False
commarg2 = False

if len(sys.argv) < 2:
    print "Please specify an executable"
    sys.exit(1)
elif len(sys.argv) == 2:
    os.environ['RECORD_MODE'] = "yes"
    print "Running in record mode"
    executable = sys.argv[1]
    print executable
    record = True
elif len(sys.argv) == 3:
    schedule = sys.argv[2]
    executable = sys.argv[1]
elif len(sys.argv) == 4:
    schedule = sys.argv[2]
    executable = sys.argv[1]
    parseExecutionMode(sys.argv[3])
elif len(sys.argv) == 5:
    schedule = sys.argv[2]
    executable = sys.argv[1]
    parseExecutionMode(sys.argv[3])
    os.environ['KLEE_VERBOSE_MODE'] = "yes"
    print "Running in verbose mode....\n"
elif len(sys.argv) > 5:
    commarg1 = sys.argv[2]
    schedule = sys.argv[3]
    executable = sys.argv[1]
    parseExecutionMode(sys.argv[4])
    os.environ['KLEE_VERBOSE_MODE'] = "yes"
    print "Running in verbose mode....\n"
    

#find out what shell the user is using
shell = os.environ['SHELL']
res = shell.split("/")
sh = res[-1]


absp = os.path.abspath('../bin')


#Set environment variable so that schedule can be accessed dynamically
if not record:
    schedloc = os.path.abspath('schedules');
    schedp = schedloc + "/" + schedule
    print schedp
    os.environ['KLEE_SCHED'] = schedp




#Set up the LD_PRELOAD variable to enable dynamic function interposition
ld_prel = absp + "/libintercept.so"

print "\nCalling executable....\n"
t = sh + " -c env "
if(not commarg1 and not commarg2):
    print "no command line args"
    path = "LD_PRELOAD=\"" + ld_prel + "\" " + "./" + executable
    complete = t + path
    os.system(path)
else:
     path = "LD_PRELOAD=\"" + ld_prel + "\" " + "./" + executable + " " + commarg1
     complete = t + path
     os.system(path)
