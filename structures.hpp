#include <vector>
#include <string>
#include <iostream>
#include <cassert>

typedef std::string Attribute;

struct Relation {
	std::string name;
	std::vector<Attribute> attributes;
};

std::ostream& operator<<(std::ostream& stream, const Relation& rel) {
	stream << rel.name << '(';
	bool first{true};
	for (const auto& attr : rel.attributes) {
		if (first) {
			first = false;
		} else {
			stream << ',';
		}
		stream << attr;
	}
	return stream;
}

using Schema = std::vector<Relation>;

std::ostream& operator<<(std::ostream& stream, const Schema& sc) {
	for (const auto& rel : sc) {
		stream << rel << std::endl;
	}
	return stream;
}

struct Value {
	typedef std::string variable_t;
	typedef int constant_t;
	enum class Type {VARIABLE, CONSTANT};
	
	Value(const variable_t& var) : type{Type::VARIABLE}, var_{var} {}
	Value(constant_t cons) : type{Type::CONSTANT}, cons_{cons} {}
	
	const Type type;
	const variable_t& asVariable() const {
		assert(type == Type::VARIABLE);
		return var_;
	}
	constant_t asConstant() const {
		assert(type == Type::CONSTANT);
		return cons_;
	}
private:
	variable_t var_;
	constant_t cons_;
};

std::ostream& operator<<(std::ostream& stream, const Value& val) {
	if (val.type == Value::Type::CONSTANT) {
		stream << val.asConstant();
	} else {
		stream << '$' << val.asVariable();
	}
	return stream;
}

struct Atom {
	std::string name;
	std::vector<Value> args;
};

std::ostream& operator<<(std::ostream& stream, const Atom& atom) {
	stream << atom.name << '(';
	bool first{true};
	for (const auto& attr : atom.args) {
		if (first) {
			first = false;
		} else {
			stream << ',';
		}
		stream << attr;
	}
	return stream;
}

using Query = std::vector<Atom>;

std::ostream& operator<<(std::ostream& stream, const Query& query) {
	bool first{true};
	for (const auto& atom : query) {
		if (first) {
			first = false;
		} else {
			stream << " ^ ";
		}
		stream << atom;
	}
	return stream;
}

struct Tgd {
	Query from;
	Query to;
};

std::ostream& operator<<(std::ostream& stream, const Tgd& tgd) {
	stream << tgd.from << "  ->  " << tgd.to;
	return stream;
}

using Mapping = std::vector<Tgd>;

std::ostream& operator<<(std::ostream& stream, const Mapping& mapping) {
	for (const auto& tgd : mapping) {
		stream << tgd << std::endl;
	}
	return stream;
}

struct FileContent {
	Schema source;
	Schema target;
	Mapping mapping;
};