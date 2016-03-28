OPT= -g
STD= -std=c++11

all: main

main: main.o structures.o utils.o parsing/parser.o
	g++ $(OPT) $(STD) -std=c++11 main.o structures.o utils.o parsing/parser.o -o main

main.o: main.cpp structures.hpp parsing/struct_builder.hpp
	g++ $(OPT) $(STD) -c main.cpp

structures.o: structures.cpp structures.hpp utils.hpp
	g++ $(OPT) $(STD) -c structures.cpp

utils.o: utils.cpp utils.hpp
	g++ $(OPT) $(STD) -c utils.cpp

parsing/parser.o:
	cd parsing; make parser.o

.PHONY: parsing/parser.o clean

clean:
	rm -f *.o
	cd parsing; make clean
