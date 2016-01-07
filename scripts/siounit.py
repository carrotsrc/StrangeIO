#!/bin/python

# Script: siounit.py
# License: MIT
# Copyright 2016, Charlie Fyvie-Gauld
# 
# This script to creates source and header files
# for a StrangeIO unit based on the template files
# provided with the script. The generate files are
# placed in the current working directory


import os
import sys
from os import environ, getcwd
from subprocess import call

sz = len(sys.argv)
if sz < 3:
	if sz == 1:
		print("Usage:\n\tsiounit.py NAME [mainline|step|combine|dispatch]")
	elif sys.argv[1] == "-v" or sys.argv[1] == "--version":
		print("StrangeIO template generator\nv1.0")
	else:
		print("Usage:\n\tsiounit.py NAME [mainline|step|combine|dispatch]")
	exit();


if not "STRANGEFW" in environ:
    print("Error: environement STRANGEFW does not exist\n\n\tTry 'export STRANGEFW=path/to/framework'")
    exit()

cwd = os.getcwd()

type_list = ["step","dispatch"]

fw = environ['STRANGEFW']
tmpl = "siostep.tpl"
unit = sys.argv[1]

if sz == 3:
	t = sys.argv[2]
	if not t in type_list:
		print("Invalid template selected")
		print("Usage:\n\tsiounit.py NAME [mainline|step|combine|dispatch]")
		exit()
	tmpl = "sio" + t + ".tpl"

with open(fw+"/scripts/"+tmpl) as f:
	lines = [line.rstrip('\n') for line in f.readlines()]

header = []
source = []
state = 0
for l in lines:
	if l == "@HEADER@":
		state = 1
		continue
	if l == "@SOURCE@":
		state = 2
		continue

	l = l.replace("@UNIT@", unit)
	l = l.replace("@DEF@", unit.upper())
	if state == 1:
		header.append(l)
	elif state == 2:
		source.append(l)


with open(cwd+"/"+unit+".cpp", 'w') as f:
	for line in source:
	  f.write("%s\n" % line)

with open(cwd+"/"+unit+".hpp", 'w') as f:
	for line in header:
	  f.write("%s\n" % line)

print("Generation: \033[1;32mOk\033[1;m")
