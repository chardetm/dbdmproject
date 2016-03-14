OPT= -g
STD= -std=c++11

all: parser.tab.o lex.yy.o struct_builder.o

parser.tab.cpp parser.tab.hpp: parser.ypp
	bison -d parser.ypp
	
lex.yy.cc: parser.lex
	flex++ --stdinit parser.lex
	
parser.tab.o: parser.tab.cpp global.hpp parser.tab.hpp
	g++ $(OPT) $(STD) -Wno-write-strings -c parser.tab.cpp

lex.yy.o: lex.yy.cc global.hpp parser.tab.hpp
	g++ $(OPT) $(STD) -c lex.yy.cc
	
struct_builder.o: struct_builder.cpp struct_builder.hpp ../structures.hpp
	g++ $(OPT) $(STD) -c struct_builder.cpp

clean:
	rm -f parser.tab.cpp parser.tab.hpp lex.yy.cc parser.tab.o lex.yy.o struct_builder.o

.PHONY: clean
