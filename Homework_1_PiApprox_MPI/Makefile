FILENAME   = PiApproximator
COMPILER   = mpicc
PROCESSES  = 8
SOURCES    = ./src/*.c
FLAGS      = -g -Wall -Wextra -lm


LINTFILTERS = $\
	-build/c++11,$\
	-build/header_guard,$\
	-build/include_subdir,$\
	-readability/casting,$\
	-runtime/references

$(FILENAME): clean
	$(COMPILER) ./src/$(FILENAME).c $(FLAGS) -o ./bin/$(FILENAME)

run: $(FILENAME)
	mpirun -n $(PROCESSES) --oversubscribe ./bin/$(FILENAME)

lint:
	cpplint --filter=$(LINTFILTERS) $(SOURCES)

clean:
	-rm -r -f ./bin/*