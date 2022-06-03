LD=g++
CXX = g++
CXXFLAGS=-Wall -pedantic -std=c++17 -g

all: galrene

galrene: main.o CGameStateManager.o CMenu.o CGameSettings.o CSaver.o CConfigParser.o CPlayer.o CCharacter.o CDeck.o CCard.o CAttack.o CDefense.o CPassive.o CSpecial.o
	$(LD) -o $@ $^ -lncurses

deps:
	$(CXX) -MM *cpp > Makefile.d

clean:
	rm -f *.o *~ galrene

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include Makefile.d