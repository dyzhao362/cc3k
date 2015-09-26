CXX = g++
CXXFLAGS = -Wall
EXEC = cc3k
OBJECTS = main.o floor.o character.o event.o tile.o stringconverter.o rng.o mob.o player.o item.o gold.o treasure.o potion.o bapotion.o bdpotion.o phpotion.o rhpotion.o wapotion.o wdpotion.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
