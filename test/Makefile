CXXFLAG = -std=c++11 -I../
OPTFLAG = -O3 -DNDEBUG

default: a64.out a32.out

clean32:
	rm -f *32.o *32.out
clean64:
	rm -f *64.o *64.out

URL = https://raw.githubusercontent.com/catchorg/Catch2/v2.13.3/single_include/catch2/catch.hpp
OS = $(shell uname -s)
ifeq ($(OS),Linux)
catch.hpp:
	wget $(URL)
endif
ifeq ($(OS),Darwin)
catch.hpp:
	curl -o catch.hpp $(URL)
endif

main.32.o: main.cc catch.hpp
	${CXX} ${CXXFLAG} ${OPTFLAG} -m32 main.cc -c -o main.32.o
main.64.o: main.cc catch.hpp
	${CXX} ${CXXFLAG} ${OPTFLAG} -m64 main.cc -c -o main.64.o

ut.allocatable.32.o: ../FortranArray ut.allocatable.cpp catch.hpp
	${CXX} ${CXXFLAG} ${OPTFLAG} -m32 ut.allocatable.cpp -c -o ut.allocatable.32.o
ut.allocatable.64.o: ../FortranArray ut.allocatable.cpp catch.hpp
	${CXX} ${CXXFLAG} ${OPTFLAG} -m64 ut.allocatable.cpp -c -o ut.allocatable.64.o

ut.dimension.32.o: ../FortranArray ut.dimension.cpp catch.hpp
	${CXX} ${CXXFLAG} ${OPTFLAG} -m32 ut.dimension.cpp -c -o ut.dimension.32.o
ut.dimension.64.o: ../FortranArray ut.dimension.cpp catch.hpp
	${CXX} ${CXXFLAG} ${OPTFLAG} -m64 ut.dimension.cpp -c -o ut.dimension.64.o

a32.out: main.32.o ut.allocatable.32.o ut.dimension.32.o
	${CXX} ${CXXFLAG} ${OPTFLAG} -m32 *32.o -o a32.out
a64.out: main.64.o ut.allocatable.64.o ut.dimension.64.o
	${CXX} ${CXXFLAG} ${OPTFLAG} -m64 *64.o -o a64.out

test: a32.out a64.out
	./a32.out
	./a64.out
