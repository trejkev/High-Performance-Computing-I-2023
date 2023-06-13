FILENAME      = fourier_transform
MPI_COMPILER  = mpicc
GNU_COMPILER  = gcc
SOURCES       = ./src/*.c
HEADERS       = ./src/*.h
FLAGS         = -g -Wall -Wextra -lm
ASAN_FLAGS    = -fsanitize=address -fno-omit-frame-pointer
LINTFILTERS   = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

PROCESSES     = 64
THREADS       = 6
REPEAT        = 1
SAMPLING_FREQ = 16384# 2^n = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, ...}
SIGNAL_DESC   = square_50Hz# {sin_50Hz, sin_50Hz_PLUS_sin_450Hz, 4_PLUS_sin_50Hz_PLUS_sin_450Hz, sin_1Hz, cos_1Hz, square_50Hz, sawtooth_150Hz}
FOURIERTYPE   = fft#{fft, dft}

################################ FT COMPUTATION ################################

$(FILENAME): $(HEADERS)
	$(GNU_COMPILER) $(SOURCES) $(FLAGS) -o ./bin/$(FILENAME)

run:
	./bin/$(FILENAME) $(SIGNAL_DESC) $(THREADS) $(REPEAT) $(SIGNAL_DESC) $(SAMPLING_FREQ) $(FOURIERTYPE)

############################## MEMORY SANITIZERS ###############################

asan: clean $(HEADERS)
	clang $(FLAGS) $(ASAN_FLAGS) $(SOURCES) -o ./bin/$(FILENAME)

memcheck: $(FILENAME)
	valgrind ./bin/$(FILENAME) $(INPUTFILE) $(THREADS) $(REPEAT) $(SIGNAL_DESC) $(SAMPLING_FREQ) $(FOURIERTYPE)

################################ MISCELLANEOUS #################################

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES) $(HEADERS)

clean:
	-rm -f ./bin/* ./test/* 