#include <vector>
#include <string>
#include <cassert>

typedef std::string Attribute;

struct Relation {
	std::string name;
	std::vector<Attribute> attributes;
};

using Schema = std::vector<Relation>;

struct Variable;
struct Constant;

struct Value {
	typedef std::string variable_t;
	typedef int constant_t;
	enum class Type {VARIABLE, CONSTANT};
	
	Value(const variable_t& var) : type{Type::VARIABLE}, var_{var} {}
	Value(constant_t cons) : type{Type::CONSTANT}, cons_{cons} {}
	
	const Type type;
	const variable_t& asVariable() {
		assert(type == Type::VARIABLE);
		return var_;
	}
	constant_t asConstant() {
		assert(type == Type::CONSTANT);
		return cons_;
	}
private:
	variable_t var_;
	constant_t cons_;
};

struct Atom {
	std::string name;
	std::vector<Value> args;
};

using Query = std::vector<Atom>;

struct Tgd {
	Query from;
	Query to;
};

using Mapping = std::vector<Tgd>;