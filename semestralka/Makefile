LD=g++
CXX = g++
CXXFLAGS=-Wall -pedantic -std=c++17 -g

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

all: galrene

galrene: $(OBJECTS)
	$(LD) -o $@ $^ -lncurses -lstdc++fs

deps:
	$(CXX) -MM src/*cpp > Makefile.d

clean:
	rm -f build/*.o galrene
count:
	wc -l src/*
build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include Makefile.d