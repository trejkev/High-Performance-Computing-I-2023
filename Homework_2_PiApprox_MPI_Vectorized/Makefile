FILENAME      = PiApproximator
MPI_COMPILER  = mpicc
GNU_COMPILER  = gcc
PROCESSES     = 4
SOURCES       = ./src/*.c
TRAPEZOIDSQTY = 640000
REPEAT        = 1


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

AGNOSTICVECTFLAGS = $\
	-g $\
	-O3 $\
	-std=gnu11 $\
	-fstrict-aliasing $\
	-ftree-vectorize $\
	-march=native $\
	-mtune=native

LINUXVECTFLAGS =  $\
	-fopt-info-vec-optimized $\
	-fopt-info-vec-missed

MACOSVECTFLAGS = $\
	-Rpass=loop-vectorize $\
	-Rpass-missed=loop-vectorize


################################## COMPILE ALL #################################

$(FILENAME)_MACOS: clean $(FILENAME)_MPI $(FILENAME)_MPI_VECTORIZED_MACOS $(FILENAME)_VECTORIZED_MACOS

$(FILENAME)_LINUX: clean $(FILENAME)_MPI $(FILENAME)_MPI_VECTORIZED_LINUX $(FILENAME)_VECTORIZED_LINUX

############################ AGNOSTIC COMPILATIONS #############################

$(FILENAME)_MPI: ./src/timeReader.h
	$(MPI_COMPILER) ./src/$(FILENAME).c ./src/timeReader.c -o ./bin/$(FILENAME)_MPI

############################## LINUX COMPILATIONS ##############################

$(FILENAME)_MPI_VECTORIZED_LINUX: ./src/timeReader.h
	$(MPI_COMPILER) ./src/$(FILENAME).c ./src/timeReader.c $(AGNOSTICVECTFLAGS) $(LINUXVECTFLAGS) -o ./bin/$(FILENAME)_MPI_VECTORIZED

$(FILENAME)_VECTORIZED_LINUX: ./src/timeReader.h
	$(GNU_COMPILER) ./src/$(FILENAME)VectorizationCompliant.c ./src/timeReader.c $(AGNOSTICVECTFLAGS) $(LINUXVECTFLAGS) -o ./bin/$(FILENAME)_VECTORIZED

############################## MAC OS COMPILATIONS #############################

$(FILENAME)_MPI_VECTORIZED_MACOS: ./src/timeReader.h
	$(MPI_COMPILER) ./src/$(FILENAME).c ./src/timeReader.c $(AGNOSTICVECTFLAGS) $(MACOSVECTFLAGS) -o ./bin/$(FILENAME)_MPI_VECTORIZED

$(FILENAME)_VECTORIZED_MACOS: ./src/timeReader.h
	$(GNU_COMPILER) ./src/$(FILENAME)VectorizationCompliant.c ./src/timeReader.c $(AGNOSTICVECTFLAGS) $(MACOSVECTFLAGS) -o ./bin/$(FILENAME)_VECTORIZED

################################ EXECUTION FILES ###############################

run_MPI:
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(FILENAME)_MPI $(REPEAT)

run_MPI_VECTORIZED:
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(FILENAME)_MPI_VECTORIZED $(REPEAT)

run_VECTORIZED:
	./bin/$(FILENAME)_VECTORIZED $(TRAPEZOIDSQTY) $(REPEAT)

################################### CLEANERS ###################################

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -r -f ./bin/*