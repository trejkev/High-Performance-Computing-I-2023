SOURCEFILE  = exercise1
COMPILER    = mpicc
EXECUTOR    = mpirun
PROCESSES   = 8
RANKTOPRINT = 3  # Debug variable, to print the data structure evolution
ROWS        = 64 # Shall be multiple of PROCESSES
COLS        = 5  # Per design can be whatever, not necessarily equal to ROWS

$(SOURCEFILE): $(SOURCEFILE)_COMPILE
	$(EXECUTOR) -n $(PROCESSES) ./bin/$(SOURCEFILE) $(RANKTOPRINT) $(ROWS) $(COLS)

$(SOURCEFILE)_COMPILE: clean
	$(COMPILER) ./src/$(SOURCEFILE).c -o ./bin/$(SOURCEFILE)

clean:
	-rm -f ./bin/$(SOURCEFILE)