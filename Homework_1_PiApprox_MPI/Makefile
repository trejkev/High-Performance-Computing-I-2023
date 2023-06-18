FILENAME   = PiApproximator
COMPILER   = mpicc
PROCESSES  = 64
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
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(FILENAME)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm ./bin/$(FILENAME)