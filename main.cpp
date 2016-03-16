#include <iostream>

#include "structures.hpp"
#include "parsing/struct_builder.hpp"

using namespace std;

std::ostream& printFreeVariables(std::ostream& stream, const Tgd& tgd) {
	bool first{true};
	for (const auto &v : tgd.freeVariables()) {
		if (first) {
			first = false;
		} else {
			cout << ", ";
		}
		cout << '$' << v;
	}
	return stream;
}

std::ostream& printBoundVariables(std::ostream& stream, const Tgd& tgd) {
	bool first{true};
	for (const auto &v : tgd.boundVariables()) {
		if (first) {
			first = false;
		} else {
			cout << ", ";
		}
		cout << '$' << v;
	}
	return stream;
}

int main (int argc, char *argv[]) {
	FileContent& fc = parseFile();
	cerr << "====== FILE PARSED ======" << endl << endl << fc << endl << "====== END OF FILE ======" << endl;
	cout << endl;
	for (const Tgd &tgd : fc.mapping) {
		cout << tgd << endl;
		printFreeVariables (cout << "  Free variables : ", tgd) << endl;
		printBoundVariables(cout << "  Bound variables: ", tgd) << endl;
	}
	cout << endl;
	for (Tgd &tgd : fc.mapping) {
		cout << tgd << endl;
		tgd.skolemize();
		cout << "  Skolemized: " << tgd << endl;
	}
	return 0;
}