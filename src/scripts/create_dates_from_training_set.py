#!/usr/bin/env python

from os import getenv, listdir
import pickle
from pprint import pprint

dates = set()

DIR = getenv("NFP_TRAINING_SET_DIR")

for filename in listdir(DIR):
    print "Processing file %s" % filename
    FILE = open(DIR + "/" + filename)
    for i in (str(line.split(',')[-1]).strip('\n') for line in FILE.readlines()[1:]):
        dates.add(i)
    FILE.close()
 
print "Writing pickled file"   
outfile = open("data/dates.dat", "wb")
pickle.dump(dates, outfile, pickle.HIGHEST_PROTOCOL)
outfile.close()
print "Bye!"