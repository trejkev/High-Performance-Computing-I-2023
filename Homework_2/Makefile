FILENAME      = PiApproximator
MPI_COMPILER  = mpicc
GNU_COMPILER  = gcc
PROCESSES     = 64
SOURCES       = ./src/*.c
TRAPEZOIDSQTY = 500


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

VECTORIZATIONFLAGS = $\
	-g $\
	-O3 $\
	-std=gnu11 $\
	-fstrict-aliasing $\
	-ftree-vectorize $\
	-march=native $\
	-mtune=native $\
	-fopt-info-vec-optimized $\
	-fopt-info-vec-missed

$(FILENAME)_MPI:
	$(MPI_COMPILER) ./src/$(FILENAME).c -o ./bin/$(FILENAME)_MPI

$(FILENAME)_MPI_VECTORIZED:
	$(MPI_COMPILER) ./src/$(FILENAME).c $(VECTORIZATIONFLAGS) -o ./bin/$(FILENAME)_MPI_VECTORIZED

$(FILENAME)_VECTORIZED:
	$(GNU_COMPILER) ./src/$(FILENAME)VectorizationCompliant.c $(VECTORIZATIONFLAGS) -o ./bin/$(FILENAME)_VECTORIZED

run_MPI:
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(FILENAME)_MPI

run_MPI_VECTORIZED:
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(FILENAME)_MPI_VECTORIZED

run_VECTORIZED:
	./bin/$(FILENAME)_VECTORIZED $(TRAPEZOIDSQTY)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/*