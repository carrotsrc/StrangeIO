#!/bin/env python
import sys
from os import environ
from subprocess import call

def print_help():
		print("Usage: run.py [flags] [test test ...]")
		print("flags\t: Any 'catch' flags that don't require an argument")
		print("test\t: One or more of the test routines")
		print("")
		
		print("Namespace test routines:")
		print("all\t\t: Run through the entire test suite")
		print("component\t: Building, linking and cycling components")
		print("config\t\t: Loading configs and assembling racks")
		print("event\t\t: Creating and processing events")
		print("memory\t\t: Cache and cache_ptr behaviour")
		print("midi\t\t: Loading midi devices and processing signals")
		print("routine\t\t: Helper routines for sound and midi")
		print("strangeio\t: The root namespace objects like library loading")
		print("thread\t\t: Running thread pool and processing tasks")
		print("")
		print("O/S Specific:")
		print("linux\t\t: Self sustaining test rig for Linux")
		exit();

if len(sys.argv) == 1:
    print_help()

if sys.argv[1] == "-h" or sys.argv[1] == "--help":
    print_help()

if not  "STRANGEFW" in environ:
    print("Error: environement STRANGEFW does not exist\n\n\tTry 'export STRANGEFW=path/to/framework'")
    exit()

target = sys.argv[1]
environ['LD_LIBRARY_PATH'] = environ['STRANGEFW']

tags = []
flags = []
first = 1
all_found = 0

for tag in sys.argv[1:]:
	if tag[0] == "-":
		flags = flags +[tag]
		continue

	if all_found == 1:
		continue

	if first == 1:
			first = 0
			tags = ["\""]
	else:
			tags = tags + [","]
			
	if tag == "all":
		tags = []
		all_found = 1
		continue

	if tag == "strangeio":
		tags = tags + ["[strangeio]"]
	else:
		tags = tags + ["[strangeio::"+tag+"]"]
	tags = tags + ["\""]

args = ["./suite"] + flags + tags

if call(args) != 0:
		print("### Test Failed ###")
		exit(1)

print("~~~ Test completed ~~~")

