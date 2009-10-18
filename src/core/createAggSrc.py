#!/bin/env python

from pprint import pprint
import datetime

TARGET = "libnfp.cpp"

SOURCES = ('Rating.cpp',
           'RatingsManager.cpp',
           'RatingsShmSegment.cpp',
           'ShmSegment.cpp')


def addComment(msg):
    ret =  "\n"
    ret += "/*\n"
    ret += " * %s\n"
    ret += " */\n\n"
    return ret % msg
    
    
lines = dict()
           
for FILE in SOURCES:
    f = open(FILE, 'r')
    lines[FILE] = list()
    for i in f.readlines():
        lines[FILE].append(i)
    f.close()
    
#pprint(lines)
    
HEADERS = set()

for file_ in lines.keys():
    for line in lines[file_]:
        if line.startswith('#include'):
            HEADERS.add(line)
        
OUTPUT = list()

COMMENT = "This file was dynamically generated on %s" % datetime.datetime.today().strftime("%Y %m %d %H:%M:%S")

OUTPUT.append(addComment(COMMENT))

for header in HEADERS:
    OUTPUT.append(header)
    
OUTPUT.append("")

for file_ in lines.keys():
    OUTPUT.append(addComment(file_))
    for line in lines[file_]:
        if not line.startswith('#include'):
            OUTPUT.append(line)
        
out = open(TARGET, 'w')

out.writelines(OUTPUT)

out.close()
