FILENAME      = fast_fourier_transform
MPI_COMPILER  = mpicc
GNU_COMPILER  = gcc
PROCESSES     = 64
THREADS       = 6
SOURCES       = ./src/*.c
REPEAT        = 300
INPUTFILE     = ./test/sinusoid.txt


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references


$(FILENAME): clean
	$(GNU_COMPILER) $(SOURCES) -lm -o ./bin/$(FILENAME)

run: $(FILENAME)
	./bin/$(FILENAME) $(INPUTFILE) $(THREADS) $(REPEAT)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/*