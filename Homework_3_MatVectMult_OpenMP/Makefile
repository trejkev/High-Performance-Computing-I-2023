FILENAME      = MatVectMult
GNU_COMPILER  = gcc
SOURCES       = ./src/*.c
THREADS       = 8
REPEAT        = 2
AROWS         = 1500
BCOLS         = 1300
ACOLS_BROWS   = 1800


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

$(FILENAME): clean $(FILENAME)_concurrent $(FILENAME)_sequential

$(FILENAME)_concurrent: ./src/timeReader.h
	$(GNU_COMPILER) -fopenmp ./src/$(FILENAME)Concurrent.c ./src/timeReader.c -o ./bin/$(FILENAME)_concurrent

$(FILENAME)_sequential: ./src/timeReader.h
	$(GNU_COMPILER) ./src/$(FILENAME)Sequential.c ./src/timeReader.c -o ./bin/$(FILENAME)_sequential

run_concurrent:
	./bin/$(FILENAME)_concurrent $(REPEAT) $(AROWS) $(BCOLS) $(ACOLS_BROWS) $(THREADS)

run_sequential:
	./bin/$(FILENAME)_sequential $(REPEAT) $(AROWS) $(BCOLS) $(ACOLS_BROWS)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/*