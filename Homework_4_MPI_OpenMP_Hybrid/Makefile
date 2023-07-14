FILENAME   = mpi_omp_hibrid
COMPILER   = mpicc
PROCESSES  = 4
THREADS    = 3
SOURCES    = ./src/*.c
FLAGS      = -fopenmp -g -Wall -Wextra -lm -I/usr/local/opt/llvm/include


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

run: $(FILENAME)
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(FILENAME) $(THREADS)

$(FILENAME): clean
	$(COMPILER) $(SOURCES) $(FLAGS) -o ./bin/$(FILENAME)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/$(FILENAME)