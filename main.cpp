#include <iostream>

#include "structures.hpp"
#include "parsing/struct_builder.hpp"

using namespace std;

int main (int argc, char *argv[]) {
	FileContent& fc = parseFile();
	cerr << "====== FILE PARSED ======" << endl << endl << fc << endl << "====== END OF FILE ======" << endl;
	return 0;
}