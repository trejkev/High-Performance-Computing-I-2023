FILENAME   = mpi_omp_hibrid
COMPILER   = mpicc
PROCESSES  = 4
THREADS    = 3
SOURCES    = ./src/*.c


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

$(FILENAME): clean
	$(COMPILER) $(SOURCES) -fopenmp -o ./bin/$(FILENAME)

run: $(FILENAME)
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(FILENAME) $(THREADS)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/$(FILENAME)