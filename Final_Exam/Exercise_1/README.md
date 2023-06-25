# User Guide

To interact with this code, a Makefile was created, it provides all the different variables the user may need to make the code work differently.

There are only 4 variables the user should modify to bring a different behavior, these are enumerated below.

1. PROCESSES: This variable brings the capability of setting the number of processes that shall run the code.
2. RANKTOPRINT: This is a debug variable, added to give the user the capability of defining what process wants to be printed, it will print the global data structure, its self data structure before computing the information exchange, and its self data structure but after computing the information exchange.
3. ROWS: This variable defines the number of rows the global data structure will have, it shall be a multiple of PROCESSES.
4. COLS: This variable defines the number of columns the global data structure will have, per exam guidelines for simplicity it shall be the same as ROWS, however it can be any other positive integer number.

To run the code simply run _make_, it  will perform all the actions needed to do so, starting with cleaning any older execution files, compiling the code, and finally running the code with the PROCESSES, ROWS, and COLS, the user settled, and printing the RANK desired, as per RANKTOPRINT.

Note: If the user does not want to print anything, simply set it to be equal or greater than PROCESSES.
