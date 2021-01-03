CXX=g++-5
CXXFLAGS=-std=c++14 -Wall -MMD
EXEC = ctor
OBJECTS=main.o address.o dice.o game.o in.o path.o player.o subject.o textdisplay.o tile.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} ${TTM}

-include ${DEPENDS}

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
.PHONY: clean