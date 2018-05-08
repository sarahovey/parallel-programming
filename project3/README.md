This project uses a simple adding operation to 
demonstrate false sharing.

Fix 1 uses padding on cache lines (NUMPAD) to try to prevent
cores from 'banging' on the same cache lines and 
slowing down performance. We expect to see improved
performance at 7-8 spaces of padding.

Fix 2 uses a local variable to solve this problem.
It acts as a benchmark of sorts for the performance
fix 1 should be doing after 7+ spaces of padding.

Instructions.txt go into more detail.

To run:
./script.sh

This will run fix1.c and fix2.c with
1-4 threads and 0-15 spaces of padding.
They will output to their own text files.