LD=g++
CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++17 -g 
LDFLAGS=-lncurses -lstdc++fs
USERNAME = galrene

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)

all: compile doc

run: compile
	@./$(USERNAME)

compile: $(USERNAME)

$(USERNAME): $(OBJECTS)
	@mkdir -p build/
	$(LD) -o $@ $^ $(LDFLAGS)

deps:
	$(CXX) -MM src/*.cpp | sed -r 's|^(.*\.o)|build/\1|' > Makefile.d

clean:
	rm -rf build/*.o $(USERNAME) Makefile.d doc/ $(USERNAME).zip

zip : $(USERNAME).zip

$(USERNAME).zip: DOCUMENTATION.md zadani.txt prohlaseni.txt Makefile Doxyfile $(wildcard src/*) $(wildcard examples/*)
	mkdir -p .archive/$(USERNAME)/
	cp -r DOCUMENTATION.md zadani.txt prohlaseni.txt Makefile Doxyfile src/ examples/ .archive/$(USERNAME)/
	cd .archive/; zip -r ../$(USERNAME).zip $(USERNAME)/
	rm -r .archive/

count:
	@wc -l src/*

build/%.o: src/%.cpp
	@mkdir -p build/
	$(CXX) $(CXXFLAGS) -c $< -o $@

doc: doc/index.html

doc/index.html: Doxyfile DOCUMENTATION.md $(wildcard src/*)
	doxygen Doxyfile

-include Makefile.d