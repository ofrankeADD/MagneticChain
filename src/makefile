CXX = g++
CPFLAGS = -Wall -Wextra -std=c++11 -fopenmp
LIBRARYS = 

OBJECTS = vertex.o dipole.o cube.o magnChain.o metropolis.o dataFileHandling.o main.o
PROJECT = magnChain

$(PROJECT) : $(OBJECTS)
	${CXX} $(CPFLAGS) $(OBJECTS) -o $@ ${LIBRARYS}

.cpp.o :
	${CXX} -c ${CPFLAGS} $?

clean :
	rm -f $(OBJECTS)
