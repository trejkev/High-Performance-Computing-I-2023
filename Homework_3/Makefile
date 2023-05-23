FILENAME      = PMatricesVectorialMultiplier
GNU_COMPILER  = gcc
SOURCES       = ./src/*.c
REPEAT        = 10


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

$(FILENAME): clean PMatricesVectorialMultiplier_parallel PMatricesVectorialMultiplier_sequential

$(FILENAME)_parallel:
	$(GNU_COMPILER) -fopenmp $(SOURCES) -o ./bin/$(FILENAME)_parallel

$(FILENAME)_sequential:
	$(GNU_COMPILER) $(SOURCES) -o ./bin/$(FILENAME)_sequential

run_parallel:
	./bin/$(FILENAME)_parallel $(REPEAT)

run_sequential:
	./bin/$(FILENAME)_sequential $(REPEAT)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/*