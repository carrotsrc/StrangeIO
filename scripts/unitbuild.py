#!/bin/env python

import sys
from os import environ
from subprocess import call

sz = len(sys.argv)

if sz < 3:
	if sz == 1:
		print("Usage:\n\tunitbuild.py TARGET [flags] SRC1 [SRC2 SRC3]")
	elif sys.argv[1] == "-v" or sys.argv[1] == "--version":
		print("StrangeIO module build script\nv1.0")
	else:
		print("Usage:\n\tunitbuild TARGET [flags] SRC1 [SRC2 SRC3]")

	exit();

for cmd in sys.argv:
	if cmd == "clean":
		print("Build: \033[1;32mskip on clean\033[1;m")
		exit(0)

if not "STRANGEFW" in environ:
    print("Error: environement STRANGEFW does not exist\n\n\tTry 'export STRANGEFW=path/to/framework'")
    exit()
fw = environ['STRANGEFW']
target = sys.argv[1]+".rso"
print("framework: "+fw)
print("module: "+target)
print("source: "+' '.join(sys.argv[2:]))
print("Building unit(s)...")
args = ["gcc", 
		"-I"+fw+"/framework/include", 
		"-L"+fw, 
		"-std=c++14", 
		"-fPIC",
		"-shared",
		"-Wall",
		"-Wl,-z,now"]

args =  args + sys.argv[2:]

args = args + [	"-pthread", 
				"-lpthread",
				"-o"+target]

if "VERBOSE" in environ and environ['VERBOSE'] == "1":
	print(" ".join(args))

if call(args) != 0:
	print("Build: \033[1;31mFailed\033[1;m");
	exit(1)

print("Build: \033[1;32mOk\033[1;m")
exit(0)
