#!/usr/bin/python
# Author : Jaehyung You
# Description : To get a result from # of threads and # of subdivisions.

#Referenece: http://web.engr.oregonstate.edu/~mjb/cs575/Handouts/project.notes.1pp.pdf
import os
  
for t in [ 1024, 128*1024, 512*1024, 1024*1024, 4*1024*1024, 8*1024*1024, 16*1024*1024 ]:
    print "NUM_ELEMENTS = %d" % t
    for s in [ 4, 16, 32, 64, 128, 256]:
        print "LOCAL_SIZE = %d" % s
        #g++-8 -DARRAYSIZE=%d  project2.cpp -o project2 -O3 -lm -fopenmp"
        cmd = "g++-9 -DNUM_ELEMENTS=%d -DLOCAL_SIZE=%d -o first first.cpp -lm -fopenmp -framework OpenCL" % ( t, s )
        os.system( cmd )
        cmd = "./first"
        os.system( cmd )
