#!/bin/bash
#script for project 3
#Run the program for differnt numbers of threads and values of NUMPAD

#number of threads:
for t in 1 2 3 4
do
    # amount of 'padding' for fix 1
    for s in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
    do
        #run fix1 for all values of threads and padding    
        g++ -DNUMPAD=$s -DNUMTHREADS=$t fix1.c -o fix1 -lm -fopenmp 
        ./fix1
		
		#Run fix2 the same amount of times for the same amount of data
		g++ -DNUMPAD=$s -DNUMTHREADS=$t fix2.c -o fix2 -lm -fopenmp 
        ./fix2
    done
done