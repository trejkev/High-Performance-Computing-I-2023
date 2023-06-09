FILENAME      = discrete_fourier_transform
MPI_COMPILER  = mpicc
GNU_COMPILER  = gcc
PROCESSES     = 64
THREADS       = 6
SOURCES       = ./src/*.c
REPEAT        = 4
FLAGS         = -g -Wall -Wextra -lm
ASAN_FLAGS    = -fsanitize=address -fno-omit-frame-pointer
SAMPLING_FREQ = 16384# 2^n = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, ...}
SIGNAL_DESC   = square_50Hz# {sin_50Hz, sin_50Hz_PLUS_sin_450Hz, 4_PLUS_sin_50Hz_PLUS_sin_450Hz, sin_1Hz, cos_1Hz, square_50Hz, sawtooth_150Hz}
LINTFILTERS   = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

################################ FFT GENERATOR #################################

$(FILENAME): signalgen_run
	$(GNU_COMPILER) ./src/$(FILENAME).c $(FLAGS) -o ./bin/$(FILENAME)

run: $(FILENAME)
	./bin/$(FILENAME) $(SIGNAL_DESC) $(THREADS) $(REPEAT)

############################## MEMORY SANITIZERS ###############################

asan: clean
	clang $(FLAGS) $(ASAN_FLAGS) $(SOURCES) -o ./bin/$(FILENAME)

memcheck: $(FILENAME)
	valgrind ./bin/$(FILENAME) $(INPUTFILE) $(THREADS) $(REPEAT)


############################### SIGNAL GENERATOR ###############################

signalgen: clean
	$(GNU_COMPILER) ./src/signal_generator.c -lm -o ./bin/signal_generator

signalgen_run: signalgen
	./bin/signal_generator $(SIGNAL_DESC) $(SAMPLING_FREQ)

################################ MISCELLANEOUS #################################

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/* ./test/* 