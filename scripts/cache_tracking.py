#!/usr/bin/env python3
#encoding: UTF-8

# Script: cache_tracking.py
# License: MIT
# Copyright 2016, Charlie Fyvie-Gauld
# 
# This script is used for analysing the log output of
# StrangeIO with CACHE_TRACKING enabled. The log must
# be in the current working direct.
#
# The purpose of the script is to help identify and 
# analyse problems related to the use of cache_ptr.
# Currently it finds lost cache_ptrs and shows the
# flow of a cache_ptr with specified Tracking ID. It
# will also provide context for the flow of the
# tracked ids

import sys
import re
from os import environ, getcwd, path

prefixes = ["* cache_ptr", "~ cache_ptr", "# cache_ptr", "> cache_ptr"]

# Entry to the program
# Here we check command line arguments and continue running
# in the specified mode
def entry():
    if len(sys.argv) < 2 or sys.argv[1] == "-h" or sys.argv[1] == "--help":
        print_usage()
        exit()


    rmode = 0
    ctrack = -1
    ctx = 0

    args = iter(sys.argv)

    options = ["--track", "-t", "--lost", "-l", "--context", "-c" ]

    if sys.argv[1] in options:
        print_error("first argument needs to be a log file, not an option")



    for arg in args:
        if arg == "--track" or arg == "-t":
            rmode = 1;
            tmp = next(args, None)

            if tmp is None or tmp.isdigit() is False:
                print_error("No tracking id specified (" + arg + ")")

            ctrack = int(tmp)
            continue
        elif arg == "--context" or arg == "-c":
            tmp = next(args, None)

            if tmp is None or tmp.isdigit() is False:
                print_error("Number of surrounding lines unspecified (" + arg + ")")

            ctx = int(tmp)
            continue
        elif arg == "--lost" or arg == "-l":
            rmode = 0
            continue

    if rmode is 1:
        track_ptr(load_log(), ctrack, ctx)
    else:
        lost_ptr(load_log())    

# Print the usage of the command line program
def print_usage():
    print("Usage:")
    print("cache_tracking.py LOGFILE [options]")
    print("\nOptions:")
    print("\t-l, --lost: count lost cache_ptr (default)")
    print("\t-t #, --track #: track flow of cache_ptr with id #\n")
    print("\t-c #, --context #: include # surrounding lines and inbetween\n")

# Print column headers
def print_headers(headers):
    print("\033[1;36m" + headers+ "\033[1;m")

# Print an error message with red highlight
def print_error(emsg):
    print("\033[1;31mError\033[1;m: " + emsg)
    print("")
    print_usage()
    exit()

# Print title of mode
def print_mode(mode):
    print("Mode: \033[1;35m"+mode+"\033[1;m\n")

# Load a StrangeIO log file with cache_ptr
# entries in
def load_log():
    cwd = getcwd()
    cache_log = cwd+"/"+sys.argv[1]

    if not path.isfile(cache_log):
        print_error("File `"+sys.argv[1]+"` does not exist in current directory")

    with open(cache_log) as f:
        lines = [line.rstrip('\n') for line in f.readlines()]
    return lines

# Get the tracking id of the cache_ptr referenced
# in the action line
def get_tid(line):
    tid = re.sub(r'\D', "", line.split("]")[0])
    if not tid.isdigit():
        return -1
    return int(tid)


## Mode function: Lost cache_ptrs
# This mode will find any cache_ptrs that were lost
# in the flow of the system. It will count them and
# display a list of their reference numbers and
# their first owner
def lost_ptr(lines):
    print_mode("lost cache_ptr")
    active = []

    for line in lines:
        if line.startswith("* cache_ptr"):
            active.append(get_tid(line))
        if line.startswith("~ cache_ptr") or line.startswith("# cache_ptr"):
            active.remove(get_tid(line))

    lost = len(active)
    print("Lost cache_ptr total: {}\n".format(lost))
    if lost > 0:
        print_headers("id\tinitial owner")
        for tid in active:
            for line in lines:
                if line.startswith("> cache_ptr [{}]".format(tid)):
                    print("{}\t".format(tid) + line.split(' ', 4)[3])
                    break

# Prints an action line with highligting
#
# format:
# line  action  owner
#
# blue highlight: instantiation
# purple highlight: move action
# red hightlight: destructor or free
# no hightlight: contextual line
#
def print_action(lno, action, owner, active):
    sys.stdout.write("{}\t".format(lno))
    if not active:
        sys.stdout.write(action) # white
    elif action[0] == '*':
        sys.stdout.write("\033[1;34m"+action+"\033[1;m") # blue
    elif action[0] == '~' or action[0] == '#':
        sys.stdout.write("\033[1;31m"+action+"\033[1;m") #red
    else:
        sys.stdout.write("\033[1;35m"+action+"\033[1;m") # purple

    sys.stdout.write("\t"+owner+"\n")

# Find last occurance of a tracking id
def find_last(lines, tid):
    global prefixes
    last = 0

    for i, line in enumerate(lines, start=0):
        pre = line[:11]        
        if pre in prefixes and tid == get_tid(line):
            last=i
    return last

# Build a list of contextual line tuples
def build_context(n, lines):
    ctx = []
    for line in lines:
        ctx.append((n, line, False))
        n = n+1
    return ctx

# Build the list of action lines for a particular
# tracking id, as well as contextual lines if they
# are requested
def build_list(lines, tid, ctx, last):
    global prefixes
    pnts = []
    started = False
    for i, line in enumerate(lines, start=0):
        pre = line[:11]      
        if pre in prefixes and tid == get_tid(line):
            if not started:
                started = True
                pnts = pnts + build_context(i+1, lines[i-ctx:i])
                pnts.append((i+1, line, True))
            elif i == last:
                pnts.append((i+1, line, True))
                pnts = pnts + build_context(i+1, lines[i+1:i+1+ctx])
                break
            else:
                pnts.append((i+1, line, True))
        elif started and ctx > 0:
            pnts.append((i+1, line,False))

    return pnts

## Mode function: Show flow of a particular cache_ptr
# This mode will count and show all the action lines of a cache_ptr
# that are recorded in the log. If specified, it will show the 
# contextual lines surrounding the action lines
def track_ptr(lines, tid, ctx):
    print_mode("Tracking cache_ptr [{}]".format(tid))
   
    last = find_last(lines, tid)
    pnts = build_list(lines, tid, ctx, last)
  
    
    total = len([i for i, v in enumerate(pnts) if v[2] == True])
    
        
    print("Points: {}".format(total))
    print_headers("\nline\taction\t\t\towner")
    for n, p, a in pnts:
        c = p.find(']')+1
        print_action(n, p[:c], p[c+1:], a)

    print("\n")

# Run the entry function in main
if __name__ == "__main__":
    entry()
