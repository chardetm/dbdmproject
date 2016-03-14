all: parser.tab.o lex.yy.o struct_builder.o

parser.tab.cpp parser.tab.hpp: parser.ypp
	@bison -d parser.ypp
	
lex.yy.cc: parser.lex
	@flex++ parser.lex
	
parser.tab.o: parser.tab.cpp global.hpp parser.tab.hpp
	@g++ -std=c++11 -Wno-write-strings -O3 -c parser.tab.cpp

lex.yy.o: lex.yy.cc global.hpp parser.tab.hpp
	@g++ -std=c++11 -O3 -c lex.yy.cc
	
struct_builder.o: struct_builder.cpp struct_builder.hpp ../structures.hpp
	@g++ -std=c++11 -O3 -c struct_builder.cpp

clean:
	@rm -f parser.tab.cpp parser.tab.hpp lex.yy.cc parser.tab.o lex.yy.o struct_builder.o 2>/dev/null; true

.PHONY: clean
