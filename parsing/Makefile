all: parser.tab.o lex.yy.o

parser.tab.c parser.tab.h: parser.y
	@bison -d parser.y
	
lex.yy.cc: parser.lex
	@flex++ parser.lex
	
parser.tab.o: parser.tab.c global.h parser.tab.h
	@gcc -std=c11 -O3 -c parser.tab.c

lex.yy.o: lex.yy.cc global.h parser.tab.h
	@g++ -std=c++11 -O3 -c lex.yy.cc

clean:
	@rm -f parser.tab.c parser.tab.h lex.yy.cc parser.tab.o lex.yy.o 2>/dev/null; true

.PHONY: clean
