# User Guide

To use this code, a Makefile is provided, there are only two variables the user must manipulate, these are THREADS and TRIALS. 

1. THREADS define the number of threads that will concurrently execute the code, being 1 a sequential execution, and more than 1 a concurrent execution.
2. TRIALS define the number of trials to approximate $\pi$.

To run the code simply run _make_. It will perform the job of cleaning any older execution files, then it will compile the code, and finally, it will run it with the THREADS and TRIALS settled by the user in the Makefile.
