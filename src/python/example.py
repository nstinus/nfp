from nfp import *

initLogging("nfp_python_example")

try:
    import psycho
except:
    print "There's no psycho here!"
else:
    psycho.full()

#RM = RatingsManager_instance()
#RM.refreshRatingsList()

print
print "Average rate by movie:"

## Evaluate average rating per movie, the silly way...
#Movies = dict()
#current_movie_id, rates, nbrates = -1, 0, 0
#it = RM.ratings_begin()
#while it != RM.ratings_end():
#    r = it.next()
#    if current_movie_id != r.movie_id():
#        if current_movie_id != -1:
#            Movies[current_movie_id] = rates / float(nbrates)
#            rates, nbrates = 0, 0
#        current_movie_id = r.movie_id()
#    rates += r.rate()
#    nbrates += 1
#Movies[current_movie_id] = rates / float(nbrates)
#
#for m, v in Movies.iteritems():
#    print "%5d %.3f" % (m, round(v, 3))

# The c++ way...
Algo = MovieMeanAlgo()
Algo.run()
for i in range(1, RatingsManager_instance().nbSegments() + 1):
    print "%5d %.3f" % (i, round(Algo.get_predicted_rate(0, i, ''), 3))

