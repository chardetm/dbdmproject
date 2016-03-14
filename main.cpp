#include <iostream>

#include "structures.hpp"
#include "parsing/struct_builder.hpp"

int main (int argc, char *argv[]) {
	FileContent& fc = parseFile();
	std::cout << fc;
	return 0;
}