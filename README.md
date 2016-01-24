Instructions for running P-Rep replay tool.

1. To compile the tool, run Make from inside the tool folder.
2. To run an executable with the tool, place the executable in the tool folder and then invoke the tool as follows:

	python tool.py [executable] [schedule] [option] [verbose]

e.g python tool.py mytest test.schedule -t -v

will run the tool in FOLLOW_AND_TERMINATE mode and in verbose mode.

P-Rep uses tool/schedules/ as its default directory to search for schedule files. Place schedules you wish to test in this folder and specify their name for P-Rep to follow them.

Options:
	-e -> will run P-Rep in FOLLOW_AND_RESUME mode
	-t -> will run P-Rep in FOLLOW_AND_TERMINATE mode
	-n -> will run P-Rep in DEFAULT mode
	-m -> will run P-Rep in RECORD mode

To run P-Rep in RECORD mode, it is enough to simply invoke the tool as follows:
	python tool.py [executable]
this will cause P-Rep to run in RECORD mode. 