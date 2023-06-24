SOURCEFILE  = pi_mc
COMPILER    = gcc
THREADS     = 8
TRIALS      = 1000000000

$(SOURCEFILE): $(SOURCEFILE)_compile
	./bin/$(SOURCEFILE) $(THREADS) $(TRIALS)

$(SOURCEFILE)_compile: clean ./src/timeReader.h ./src/random.h
	$(COMPILER) -fopenmp ./src/*.c -o ./bin/$(SOURCEFILE)

clean:
	-rm -f ./bin/*