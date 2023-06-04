# MPI and OMP Hybrid Example

This homework consisted in the creation of a hybrid MPI-OMP implementation, the code is quite simple, it just creates N processes, each of them with M threads, and the goal is that using an MPI reduction, communicate to the root process the quantity of threads that each process is executing, add them all together (with the reduction), and print-out the result, which is the total quantity of threads running the code.

To run the code it is as simple as using 'make run', it will remove any executable file into bin directory, compile the code, and run it. To control the quantity of processes change the variable named PROCESSES into the Makefile, and also, if you want to control the quantity of threads, change the variable named THREADS in the makefile.
