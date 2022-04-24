CXX=g++
CXXFLAGS=-std=c++14 -MMD
OBJECTS=setCoverMain.o setCover.o
DEPENDS=${OBJECTS:.o=.d}
EXEC=setCoverProblems

${EXEC}: ${OBJECTS} 
	${CXX} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
