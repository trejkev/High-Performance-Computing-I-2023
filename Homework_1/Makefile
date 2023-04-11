FILENAME   = PiApproximator
COMPILER   = mpicc
PROCESSES  = 1
HEADERS    = ./src/*.h
SOURCES    = ./src/*.c


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

$(FILENAME):
	$(COMPILER) ./src/$(FILENAME).c -o ./bin/$(FILENAME)

run:
	mpirun -n $(PROCESSES) ./bin/$(FILENAME)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)