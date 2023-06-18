FILENAME      = PiApproximator
MPI_COMPILER  = mpicc
GNU_COMPILER  = gcc
PROCESSES     = 64
SOURCES       = ./src/*.c
TRAPEZOIDSQTY = 640000
REPEAT        = 300


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

$(FILENAME): $(FILENAME)_MPI $(FILENAME)_MPI_VECTORIZED $(FILENAME)_VECTORIZED

$(FILENAME)_MPI: ./src/timeReader.h
	$(MPI_COMPILER) ./src/$(FILENAME).c ./src/timeReader.c -o ./bin/$(FILENAME)_MPI

$(FILENAME)_MPI_VECTORIZED: ./src/timeReader.h
	$(MPI_COMPILER) ./src/$(FILENAME).c ./src/timeReader.c $(VECTORIZATIONFLAGS) -o ./bin/$(FILENAME)_MPI_VECTORIZED

$(FILENAME)_VECTORIZED: ./src/timeReader.h
	$(GNU_COMPILER) ./src/$(FILENAME)VectorizationCompliant.c ./src/timeReader.c $(VECTORIZATIONFLAGS) -o ./bin/$(FILENAME)_VECTORIZED

run_MPI:
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(FILENAME)_MPI $(REPEAT)

run_MPI_VECTORIZED:
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(FILENAME)_MPI_VECTORIZED $(REPEAT)

run_VECTORIZED:
	./bin/$(FILENAME)_VECTORIZED $(TRAPEZOIDSQTY) $(REPEAT)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/*