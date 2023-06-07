FILENAME      = fast_fourier_transform
MPI_COMPILER  = mpicc
GNU_COMPILER  = gcc
PROCESSES     = 64
THREADS       = 6
SOURCES       = ./src/*.c
REPEAT        = 300
INPUTFILE     = ./test/signalA.txt


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

FLAGS      = -g -Wall -Wextra

ASAN_FLAGS = -fsanitize=address -fno-omit-frame-pointer

$(FILENAME): clean
	$(GNU_COMPILER) ./src/$(FILENAME).c -g -lm -I/usr/include/apr-1.0 -lapr-1 -o ./bin/$(FILENAME)

run: $(FILENAME)
	./bin/$(FILENAME) $(INPUTFILE) $(THREADS) $(REPEAT)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/*

asan: clean
	clang $(FLAGS) $(ASAN_FLAGS) $(SOURCES) -I/usr/include/apr-1.0 -lapr-1 -o ./bin/$(FILENAME)

memcheck: $(FILENAME)
	valgrind ./bin/$(FILENAME) $(INPUTFILE) $(THREADS) $(REPEAT)


################################################################################

signalGen: clean
	$(GNU_COMPILER) ./src/signal_generator.c -lm -o ./bin/signal_generator

signalGen_run: signalGen
	./bin/signal_generator ./test/signalA.txt 1024