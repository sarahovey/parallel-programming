#!/bin/bash
#script for project 1 in CS475
#Run the program for differnt numbers of threads and nodes


#number of threads:
for t in 1 2 4 8
do
    #echo NUMT = $t
    # number of subdivisions/nodes:
    for s in 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192
    do
        #echo NUMNODES = $s
        g++ -DNUMNODES=$s -DNUMTHREADS=$t prog.c -o prog -lm -fopenmp 
        ./prog 
    done
done