#include "../structures.hpp"

namespace struct_builder {
	//TODO: Listes vides ??
	
	
	const Value& constructValue (int constant);
	const Value& constructValue (const char* str);
	
	void addValue ();
	
	const Atom& constructAtom (const char* str);
	
	void addAtom ();
	
	void changeQuery ();
	
	void constructTgd ();
	
	void addTgd ();
	
	void addAttribute (char* name);
	
	const Relation& constructRelation (char* name);
	
	void addRelation ();
	
	void changeSchema ();
	
	void buildFileContent ();

}

FileContent& parseFile();