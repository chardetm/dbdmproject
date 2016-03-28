#include <iostream>
#include <string>

extern "C" {
	#include <sqlite3.h>
}

#include "utils.hpp"
#include "structures.hpp"
#include "parsing/struct_builder.hpp"

using namespace std;

int main (int argc, char *argv[]) {
	bool verbose{false};
	std::string sqlite3_db;
	for (int i=1; i<argc; ++i) {
		if (string(argv[i]) == "-verbose") {
			verbose = true;
		} else if (string(argv[i]) == "-sqlite3") {
			sqlite3_db = argv[++i];
		} else {
			cerr << "Warning: unrecognized argument \"" << argv[i] << "\"!" << std::endl;
		}
	}
	
	FileContent& fc = parseFile();
	if (verbose) {
		cerr << "====== FILE PARSED ======" << endl << endl << fc << endl << "====== END OF FILE ======" << endl;
		cerr << endl;
	}
	
	sqlite3 *db = nullptr;
	if (sqlite3_db.length() > 0) {
		int rc = sqlite3_open(sqlite3_db.c_str(), &db);
		if (rc) {
			cerr << "Error: cannot open the database: " << sqlite3_errmsg(db) << std::endl;
			cerr << "Continuing without modifying the database." << std::endl;
			sqlite3_close(db);
			db = nullptr;
		}
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
		auto sqlstat = tgd.toSqlStatement(fc.source, fc.target);
		cout << sqlstat;
		
		if (db != nullptr) {
			char *errmsg = nullptr;
			sqlite3_exec(db, sqlstat.c_str(), nullptr, nullptr, &errmsg);
			if (errmsg != nullptr) {
				cerr << "Error while executing a SQL statement. Error message: " << errmsg << std::endl;
				cerr << "The statement was:" << std::endl << sqlstat;
				sqlite3_free(errmsg);
				errmsg = nullptr;
			}
		}
		
		if (verbose) {
			cerr << endl;
		}
	}
	
	if (db != nullptr) {
		sqlite3_close(db);
	}
	
	return 0;
}
