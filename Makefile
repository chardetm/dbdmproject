OPT= -g
STD= -std=c++11

all: main

main: main.o structures.o parsing/lex.yy.o parsing/parser.tab.o parsing/struct_builder.o
	g++ $(OPT) $(STD) -std=c++11 main.o structures.o parsing/lex.yy.o parsing/parser.tab.o parsing/struct_builder.o -o main

main.o: main.cpp structures.hpp parsing/struct_builder.hpp
	g++ $(OPT) $(STD) -c main.cpp

structures.o: structures.cpp structures.hpp
	g++ $(OPT) $(STD) -c structures.cpp

parsing/lex.yy.o parsing/parser.tab.o parsing/struct_builder.o:
	cd parsing; make

.PHONY: parsing/lex.yy.o parsing/parser.tab.o parsing/struct_builder.o clean

clean:
	rm -f *.o
	cd parsing; make clean