from nfp import *

initLogging("nfp_python_example")

try:
    import psycho
except:
    print "No module psycho found."
else:
    psycho.full()

RM = RatingsManager_instance()
RM.refreshRatingsList()

Movies = dict()

print "Looping over the ratings..."
it = RM.ratings_begin()
while it != RM.ratings_end():
    r = it.next()
    if not Movies.has_key(r.movie_id()):
        Movies[r.movie_id()] = [0, 0]
    Movies[r.movie_id()][0] += r.rate()
    Movies[r.movie_id()][1] += 1

print
print "Let's average!"
for movie_id, values in Movies.iteritems():
    print "%10d %f" % (movie_id, values[0] / float(values[1]))

