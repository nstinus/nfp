#!/usr/bin/env python

from os import getenv, listdir
import pickle
from pprint import pprint
from pylab import *

DIR = getenv("NFP_TRAINING_SET_DIR")

filename = "mv_0000001.txt"

FILE = open(DIR + "/" + filename)
data = [int(line.split(',')[-2]) for line in FILE.readlines()[1:]]
FILE.close()

hist(data, 5)
show()
 
