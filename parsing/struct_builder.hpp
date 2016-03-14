#include "../structures.hpp"

namespace struct_builder {
	//TODO: Listes vides ??
	
	
	void constructValue (int constant);
	void constructValue (const char* str);
	
	void addValue ();
	
	void constructAtom (const char* str);
	
	void addAtom ();
	
	void changeQuery ();
	
	void constructTgd ();
	
	void addTgd ();
	
	void addAttribute (char* name);
	
	void constructRelation (char* name);
	
	void addRelation ();
	
	void changeSchema ();
	
	void buildFileContent ();

}

FileContent& parseFile();