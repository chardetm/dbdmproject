#include <vector>
#include <array>
#include <memory>
#include <cassert>

#include "../structures.hpp"

extern int yyparse();

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
	

	void constructValue (int constant) {
		val.set(static_cast<Value::constant_t>(constant));
	}

	void constructValue (const char* str) {
		assert(*str != '\0');
		val.set(static_cast<Value::variable_t>(static_cast<Value::variable_t>(str+1)));
	}
	
	void addValue () {
		args.push_back(val);
	}
	
	void constructAtom (const char* str) {
		atom.args.swap(args);
		args.clear();
		atom.name = std::string(str);
	}
	
	void addAtom () {
		queries.at(qs?1:0).push_back(std::move(atom));
		atom = Atom();
	}
	
	void changeQuery () {
		qs = !qs;
	}
	
	void constructTgd () {
		tgd.from.swap(queries.at(1));
		queries.at(1).clear();
		tgd.to.swap(queries.at(0));
		queries.at(0).clear();
	}
	
	void addTgd () {
		tgds.push_back(std::move(tgd));
		tgd = Tgd();
	}
	
	void addAttribute (char* name) {
		attributes.push_back(static_cast<Attribute>(name));
	}
	
	void constructRelation (char* name) {
		relation.name = static_cast<std::string>(name);
		relation.attributes.swap(attributes);
		attributes.clear();
	}
	
	void addRelation () {
		schemas.at(qs?1:0).push_back(std::move(relation));
		relation = Relation();
	}
	
	void changeSchema () {
		ss = !ss;
	}
	
	void buildFileContent () {
		fc.source.swap(schemas.at(1));
		schemas.at(1).clear();
		fc.target.swap(schemas.at(0));
		schemas.at(0).clear();
		fc.mapping.swap(tgds);
		tgds.clear();
	}

}


FileContent& parseFile() {
	yyparse();
	return struct_builder::fc;
}