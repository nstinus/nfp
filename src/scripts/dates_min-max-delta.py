#!/usr/bin/env python

import pickle
from datetime import datetime

file = open('data/dates.dat', 'rb')
data = pickle.load(file)
dates = [datetime.strptime(i, "%Y-%m-%d") for i in data]

print "Min", min(dates)
print "Max", max(dates)
print "Delta:", max(dates) - min(dates)

