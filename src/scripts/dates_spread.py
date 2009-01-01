import sqlite3
from datetime import datetime


conn = sqlite3.connect('data/nfp.sqlite')
cur = conn.cursor()
cur.execute("select distinct date from rating limit 10")

data = map(lambda x: datetime(*map(lambda y: int(y), x[0].split('-'))).toordinal(), cur.fetchall())

conn.close()

MIN = int(min(data))
MAX = int(max(data))

print "MIN %d" % MIN
print "MAX %d" % MAX
print "NB  %d" % int(MAX-MIN)