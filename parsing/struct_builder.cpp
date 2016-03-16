#include <vector>
#include <array>
#include <memory>
#include <cassert>

#include "../structures.hpp"

extern int yyparse();
extern bool verbose_parser;

namespace struct_builder {
	
	
	Value val;
	std::vector<Value> args;
	Atom atom;
	std::array<Query, 2> queries;
	bool qs{true};
	Tgd tgd;
	Mapping tgds;
	std::vector<Attribute> attributes;
	Relation relation;
	std::array<Schema, 2> schemas;
	bool ss{true};
	FileContent fc;
	

	const Value& constructValue (int constant) {
		val.set(static_cast<constant_t>(constant));
		return val;
	}

	const Value& constructValue (const char* str) {
		assert(*str != '\0');
		val.set(static_cast<variable_t>(static_cast<variable_t>(str+1)));
		return val;
	}
	
	void addValue () {
		args.push_back(val);
	}
	
	const Atom& constructAtom (const char* str) {
		atom.args.swap(args);
		args.clear();
		atom.name = std::string(str);
		return atom;
	}
	
	void addAtom () {
		queries.at(qs?1:0).push_back(std::move(atom));
		atom = Atom();
	}
	
	void changeQuery () {
		qs = !qs;
	}
	
	const Tgd& constructTgd () {
		tgd.from.swap(queries.at(0));
		queries.at(0).clear();
		tgd.to.swap(queries.at(1));
		queries.at(1).clear();
		return tgd;
	}
	
	void addTgd () {
		tgds.push_back(std::move(tgd));
		tgd = Tgd();
	}
	
	void addAttribute (char* name) {
		attributes.push_back(static_cast<Attribute>(name));
	}
	
	const Relation& constructRelation (char* name) {
		relation.name = static_cast<std::string>(name);
		relation.attributes.swap(attributes);
		attributes.clear();
		return relation;
	}
	
	void addRelation () {
		schemas.at(ss?1:0).push_back(std::move(relation));
		relation = Relation();
	}
	
	void changeSchema () {
		ss = !ss;
	}
	
	const FileContent& buildFileContent () {
		fc.source.swap(schemas.at(0));
		schemas.at(0).clear();
		fc.target.swap(schemas.at(1));
		schemas.at(1).clear();
		fc.mapping.swap(tgds);
		tgds.clear();
		return fc;
	}

}


FileContent& parseFile(bool verbose) {
	verbose_parser = verbose;
	if (verbose) {
		std::cerr << "===== VERBOSE PARSING ======" << std::endl << std::endl;
	}
	yyparse();
	return struct_builder::fc;
}