#!/bin/env python
import sys
from os import environ
from subprocess import call

def print_help():
        print("Usage: run [test selection]")
        print("Options")
        print("all\t: Run all tests across full architecture")
        print("(test)\t: Run one of the test routines")
        exit();

if len(sys.argv) == 1:
    print_help()

if sys.argv[1] == "-h" or sys.argv[1] == "--help":
    print_help()

if not environ.has_key('STRANGEFW'):
    print "Error: environement STRANGEFW does not exist\n\n\tTry 'export STRANGEFW=path/to/framework'"
    exit()

target = sys.argv[1]
environ['LD_LIBRARY_PATH'] = environ['STRANGEFW']
if target == "all":
    print("Running all tests")
    print("~~~ Tests completed ~~~")
else:
    args = ["./"+target] + sys.argv[2:]
    print(" ".join(args))
    call(args)
    print("~~~ Test completed ~~~")

