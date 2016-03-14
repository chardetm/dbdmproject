#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <cassert>

typedef std::string Attribute;

struct Relation {
	std::string name;
	std::vector<Attribute> attributes;
};

std::ostream& operator<<(std::ostream& stream, const Relation& rel);

using Schema = std::vector<Relation>;

std::ostream& operator<<(std::ostream& stream, const Schema& sc);

struct Value {
	typedef std::string variable_t;
	typedef int constant_t;
	enum class Type {VARIABLE, CONSTANT, NO_TYPE};
	
	Value() : type_{Type::NO_TYPE} {}
	
	Value(const variable_t& var) : type_{Type::VARIABLE}, var_{var} {}
	Value(constant_t cons) : type_{Type::CONSTANT}, cons_{cons} {}
	
	void set(const variable_t& var) {
		type_ = Type::VARIABLE;
		var_ = var;
	}
	
	void set(constant_t cons) {
		type_ = Type::CONSTANT;
		cons_ = cons;
	}
	
	Type type() const { return type_; }
	const variable_t& asVariable() const {
		assert(type_ == Type::VARIABLE);
		return var_;
	}
	constant_t asConstant() const {
		assert(type_ == Type::CONSTANT);
		return cons_;
	}
private:
	Type type_;
	variable_t var_;
	constant_t cons_;
};

std::ostream& operator<<(std::ostream& stream, const Value& val);

struct Atom {
	std::string name;
	std::vector<Value> args;
};

std::ostream& operator<<(std::ostream& stream, const Atom& atom);

using Query = std::vector<Atom>;

std::ostream& operator<<(std::ostream& stream, const Query& query);

struct Tgd {
	Query from;
	Query to;
};

std::ostream& operator<<(std::ostream& stream, const Tgd& tgd);

using Mapping = std::vector<Tgd>;

std::ostream& operator<<(std::ostream& stream, const Mapping& mapping);

struct FileContent {
	Schema source;
	Schema target;
	Mapping mapping;
};

std::ostream& operator<<(std::ostream& stream, const FileContent& fc);