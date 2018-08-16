CXXFLAG = -std=c++11
# OPTFLAG = -O3 -D NDEBUG
OPTFLAG = -O3

default: a64 a32

clean32:
	rm -f *.32.o *.32.out
clean64:
	rm -f *.64.o *.64.out

main.32.o: main.cc
	${CXX} ${CXXFLAG} ${OPTFLAG} -m32 main.cc -c -o main.32.o
main.64.o: main.cc
	${CXX} ${CXXFLAG} ${OPTFLAG} -m64 main.cc -c -o main.64.o

ut.allocatable.32.o: FortranArray.h ut.allocatable.cpp
	${CXX} ${CXXFLAG} ${OPTFLAG} -m32 ut.allocatable.cpp -c -o ut.allocatable.32.o
ut.allocatable.64.o: FortranArray.h ut.allocatable.cpp
	${CXX} ${CXXFLAG} ${OPTFLAG} -m64 ut.allocatable.cpp -c -o ut.allocatable.64.o

ut.dimension.32.o: FortranArray.h ut.dimension.cpp
	${CXX} ${CXXFLAG} ${OPTFLAG} -m32 ut.dimension.cpp -c -o ut.dimension.32.o
ut.dimension.64.o: FortranArray.h ut.dimension.cpp
	${CXX} ${CXXFLAG} ${OPTFLAG} -m64 ut.dimension.cpp -c -o ut.dimension.64.o

a32: main.32.o ut.allocatable.32.o ut.dimension.32.o
	${CXX} ${CXXFLAG} ${OPTFLAG} -m32 *.32.o -o a32.out
a64: main.64.o ut.allocatable.64.o ut.dimension.64.o
	${CXX} ${CXXFLAG} ${OPTFLAG} -m64 *.64.o -o a64.out

test: a32 a64
	./a32.out
	./a64.out
