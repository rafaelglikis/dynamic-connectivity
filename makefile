CC=g++
FLAGS=-O3 -fopenmp -std=c++11

SOURCES=src/main.cpp \
incl/graph_types/DynamicGraph.h src/graph_types/DynamicGraph.cpp \
incl/graph_types/Relatives.h incl/graph_types/UndirectedGraph.h \
src/actions/Action.cpp incl/actions/Action.h \
src/actions/Delete.cpp incl/actions/Delete.h \
src/actions/Insert.cpp incl/actions/Insert.h \
incl/tests/Test.h \
src/tests/DynamicGraphTest.cpp incl/tests/DynamicGraphTest.h \
src/tests/ActionTest.cpp incl/tests/ActionTest.h \
src/benchmarks/Benchmark.cpp incl/benchmarks/Benchmark.h \
src/benchmarks/PathBenchmark.cpp incl/benchmarks/PathBenchmark.h \
src/benchmarks/TreeBenchmark.cpp incl/benchmarks/TreeBenchmark.h \
src/benchmarks/RandomGraphBenchmark.cpp incl/benchmarks/RandomGraphBenchmark.h \
src/benchmarks/PathCompleteGraphsBenchmark.cpp incl/benchmarks/PathCompleteGraphsBenchmark.h \
src/utilities/Utilities.cpp incl/utilities/Utilities.h \
src/examples/example.cpp incl/examples/example.h

OBJECTS=$(SOURCES:src/%.cpp=build/%.o)

BIN=bin/dc
INCL='/usr/include/boost'

exe: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(OBJECTS) $(FLAGS) -o $(BIN)

build/%.o: src/%.cpp incl/%.h
	$(CC) -c $(FLAGS) -I $(INCL) $< -o $@

build/main.o: src/main.cpp
	$(CC) -c $(FLAGS) -I $(INCL)  $< -o $@

clean:
	rm build/*.o
	rm build/*/*.o
	rm $(BIN)