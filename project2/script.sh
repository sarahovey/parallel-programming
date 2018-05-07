#!/bin/bash

#script for project 2 in CS475
#Run the program for fine and coarse grained parallelism, 
#static and dynamic scheduling, and different numbers of nodes


scheduleTypes='S_STATIC S_DYNAMIC'
granTypes='FINE COARSE'

#Granularity types
for g in $granTypes
do
    for s in $scheduleTypes
    do
        #number of threads:
        for t in 1 2 3 4 5 6 7 8
        do
            g++ -DNUMTHREADS=$t -DSCHEDULE=$s -DGRAN=$g prog.c -o prog -lm -fopenmp 
            ./prog 
        done
    done 
done 
