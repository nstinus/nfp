import commands, string, os, pprint


qualifying_file_name = os.getenv("NFP_ROOT") + "/original_data/probe.txt"

outFile = "./probe-out.txt"

qFile = open(qualifying_file_name, 'r')
outFile = open(outFile, 'w')

movie_rates = dict()

for i in (j. split() for j in commands.getoutput("nfpshm info | awk '{print $1, $4}'").split("\n")[2:-2]):
     movie_rates[string.atoi(i[0])] = string.atof(i[1])
     
pprint.pprint(movie_rates)
    
for line in qFile:
    line = line.strip("\n")
    if line.find(":") != -1:
        movie_id = string.atoi(line[:-1])
        print "%20s --> %d:" % (line, movie_id)
        print >>outFile, "%d:" % movie_id
    else:
        print "%20s --> %5.3f" % (line, movie_rates[movie_id])
        print >>outFile, "%5.3f" % movie_rates[movie_id]

qFile.close()    
outFile.close()
