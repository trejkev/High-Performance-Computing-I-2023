FILENAME      = PMatricesVectorialMultiplier
GNU_COMPILER  = gcc
SOURCES       = ./src/*.c
REPEAT        = 300


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

$(FILENAME): clean
	$(GNU_COMPILER) -fopenmp $(SOURCES) -o ./bin/$(FILENAME)

run:
	./bin/$(FILENAME)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -f ./bin/*