#include <iostream>

#include "structures.hpp"
#include "parsing/struct_builder.hpp"

using namespace std;

template <class SetType>
std::ostream& printSet(std::ostream& stream, const SetType& set, string prefix="", string suffix="") {
	bool first{true};
	for (const auto &v : set) {
		if (first) {
			first = false;
		} else {
			cout << ", ";
		}
		cout << prefix << v << suffix;
	}
	return stream;
}

template <class MapType>
std::ostream& printMapKeys(std::ostream& stream, const MapType& map, string prefix="", string suffix="") {
	bool first{true};
	for (const auto &p : map) {
		if (first) {
			first = false;
		} else {
			cout << ", ";
		}
		cout << prefix << p.first << suffix;
	}
	return stream;
}

int main (int argc, char *argv[]) {
	FileContent& fc = parseFile();
	cerr << "====== FILE PARSED ======" << endl << endl << fc << endl << "====== END OF FILE ======" << endl;
	cout << endl;
	for (Tgd &tgd : fc.mapping) {
		cout << "==" << endl << tgd << endl;
                auto vars = tgd.freeBoundVariables();
		printSet(cout << "  Free variables : ", vars.free , "$") << endl;
		printSet(cout << "  Bound variables: ", vars.bound, "$") << endl;
		cout << "== Skolemize ==" << endl;
		tgd.skolemize();
		cout << tgd << endl;
		cout << "  Bounds: ";
		for (const auto &p : tgd.varBounders()) {
			cout << '{' << p.first << ": " << p.second.rel << ',' << p.second.id << "} "; 
		}
		cout << endl;
		cout << "== SQL ==" << endl;
		cout << tgd.toSqlStatement(fc.source, fc.target);
		cout << endl;
	}
	return 0;
}
