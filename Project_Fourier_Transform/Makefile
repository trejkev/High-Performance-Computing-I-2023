PROJECTNAME   = fourier_transform
MPI_COMPILER  = mpicc
GNU_COMPILER  = gcc
SOURCES       = ./src/*.c
HEADERS       = ./src/*.h
FLAGS         = -fopenmp -g -Wall -Wextra -lm
ASAN_FLAGS    = -fsanitize=address -fno-omit-frame-pointer
LINTFILTERS   = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

PROCESSES     = 3
THREADS       = 12
REPEAT        = 8
SAMPLING_FREQ = 16384# 2^n = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, ...}
SIGNAL_DESC   = square_50Hz# {sin_50Hz, sin_50Hz_PLUS_sin_450Hz, 4_PLUS_sin_50Hz_PLUS_sin_450Hz, sin_1Hz, cos_1Hz, square_50Hz, sawtooth_150Hz}
FOURIERTYPE   = dft#{fft, dft}

################################ FT COMPUTATION ################################

$(PROJECTNAME): clean $(HEADERS)
	$(MPI_COMPILER) $(SOURCES) $(FLAGS) -o ./bin/$(PROJECTNAME)

run:
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(PROJECTNAME) $(SIGNAL_DESC) $(THREADS) $(REPEAT) $(SIGNAL_DESC) $(SAMPLING_FREQ) $(FOURIERTYPE)

############################## MEMORY SANITIZERS ###############################

asan: clean $(HEADERS)
	clang $(FLAGS) $(ASAN_FLAGS) $(SOURCES) -o ./bin/$(PROJECTNAME)

memcheck: $(PROJECTNAME)
	valgrind ./bin/$(PROJECTNAME) $(SIGNAL_DESC) $(THREADS) $(REPEAT) $(SIGNAL_DESC) $(SAMPLING_FREQ) $(FOURIERTYPE) --leak-check=full

################################ MISCELLANEOUS #################################

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES) $(HEADERS)

clean:
	-rm -f ./bin/* ./test/* 