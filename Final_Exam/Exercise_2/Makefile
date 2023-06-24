SOURCEFILE  = pi_mc
COMPILER    = gcc
THREADS     = 1

$(SOURCEFILE): $(SOURCEFILE)_compile
	./bin/$(SOURCEFILE) $(THREADS)

$(SOURCEFILE)_compile: clean ./src/timeReader.h ./src/random.h
	$(COMPILER) -fopenmp ./src/*.c -o ./bin/$(SOURCEFILE)

clean:
	-rm -f ./bin/*