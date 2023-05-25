FILENAME      = MatricesVectorialMultiplier
GNU_COMPILER  = gcc
SOURCES       = ./src/*.c
REPEAT        = 10
AROWS         = 5
BCOLS         = 3
ACOLS_BROWS   = 7


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

$(FILENAME): clean MatricesVectorialMultiplier_parallel MatricesVectorialMultiplier_sequential

$(FILENAME)_parallel:
	$(GNU_COMPILER) -fopenmp $(SOURCES) -o ./bin/$(FILENAME)_parallel

$(FILENAME)_sequential:
	$(GNU_COMPILER) $(SOURCES) -o ./bin/$(FILENAME)_sequential

run_parallel:
	./bin/$(FILENAME)_parallel $(REPEAT) $(AROWS) $(BCOLS) $(ACOLS_BROWS)

run_sequential:
	./bin/$(FILENAME)_sequential $(REPEAT) $(AROWS) $(BCOLS) $(ACOLS_BROWS)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/*