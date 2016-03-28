#include <iostream>
#include <string>
#include "utils.hpp"
#include "structures.hpp"
#include "parsing/struct_builder.hpp"

using namespace std;

int main (int argc, char *argv[]) {
	bool verbose{false};
	std::string sqlite3;
	for (int i=1; i<argc; ++i) {
		if (string(argv[i]) == "-verbose") {
			verbose = true;
		} else if (string(argv[i]) == "-sqlite3") {
			sqlite3 = argv[++i];
		} else {
			cerr << "Warning: unrecognized argument \"" << argv[i] << "\"!" << std::endl;
		}
	}
	
	FileContent& fc = parseFile();
	if (verbose) {
		cerr << "====== FILE PARSED ======" << endl << endl << fc << endl << "====== END OF FILE ======" << endl;
		cerr << endl;
	}
	for (Tgd &tgd : fc.mapping) {
		if (verbose) {
			cerr << "==" << endl << tgd << endl;
			auto vars = tgd.freeBoundVariables();
			printSet(cerr << "  Free variables : ", vars.free , "$") << endl;
			printSet(cerr << "  Bound variables: ", vars.bound, "$") << endl;
			cerr << "== Skolemize ==" << endl;
		}
		tgd.skolemize();
		if (verbose) {
			cerr << tgd << endl;
			cerr << "  Bounds: ";
			for (const auto &p : tgd.varBounders()) {
				cerr << '{' << p.first << ": " << p.second.rel << ',' << p.second.id << "} "; 
			}
			cerr << endl;
			cerr << "== SQL ==" << endl;
			cerr << "  -- to stdout --" << endl;
		}
		cout << tgd.toSqlStatement(fc.source, fc.target);
		if (verbose) {
			cerr << endl;
		}
	}
	return 0;
}
