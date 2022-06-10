LD=g++
CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++17 -g 
LDFLAGS=-lncurses -lstdc++fs
USERNAME = galrene

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

all: compile

run: compile
	@./$(USERNAME)

compile: $(USERNAME)

$(USERNAME): $(OBJECTS)
	@mkdir -p build/
	$(LD) -o $@ $^ $(LDFLAGS)

deps:
	$(CXX) -MM src/*.cpp | sed -r 's|^(.*\.o)|build/\1|' > Makefile.d

clean:
	rm -f build/*.o $(USERNAME) Makefile.d

count:
	@wc -l src/*

build/%.o: src/%.cpp
	@mkdir -p build/
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include Makefile.d