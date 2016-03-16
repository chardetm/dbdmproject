#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <cassert>

typedef std::string variable_t;
typedef int constant_t;

struct Value;
typedef std::unordered_map<variable_t, Value> substitution_t;

typedef std::string Attribute;

struct Relation {
	std::string name;
	std::vector<Attribute> attributes;
	
	std::vector<Attribute>::size_type arity() { return attributes.size(); }
};

std::ostream& operator<<(std::ostream& stream, const Relation& rel);

using Schema = std::vector<Relation>;

std::ostream& operator<<(std::ostream& stream, const Schema& sc);


struct Value {
	enum class Type {VARIABLE, CONSTANT, SKOLEM, NO_TYPE};
	struct SkolemReturnType {
		const std::string& name;
		const std::string& free_name;
		const std::vector<Value>& values;
	};
	
	Value() : type_{Type::NO_TYPE} {}
	
	// Variable constructor
	Value(variable_t var) : type_{Type::VARIABLE}, var_{std::move(var)} {}
	// Constant constructor
	Value(constant_t cons) : type_{Type::CONSTANT}, cons_{cons} {}
	// Skolem term constructor
	Value(std::string name, std::string free_name, std::vector<Value> values) : type_{Type::SKOLEM}, var_{free_name}, name_{std::move(name)}, values_{std::move(values)} {}
	
	// Variable setter
	void set(variable_t var) {
		type_ = Type::VARIABLE;
		var_ = std::move(var);
		name_.clear();
		values_.clear();
	}
	
	// Constant setter
	void set(constant_t cons) {
		type_ = Type::CONSTANT;
		cons_ = cons;
		var_.clear();
		name_.clear();
		values_.clear();
	}
	
	// Skolem term setter
	void set(std::string name, std::string free_name, std::vector<Value> values) {
		type_ = Type::SKOLEM;
		var_ = std::move(free_name);
		name_ = std::move(name);
		values_ = std::move(values);
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
	const SkolemReturnType asSkolem() const {
		assert(type_ == Type::SKOLEM);
		return {name_, var_, values_};
	}
	
	void skolemize(const substitution_t& substitution) {
		if (type_ == Type::VARIABLE) {
			auto it = substitution.find(var_);
			if (it != substitution.end()) { // Free variable
				(*this) = (it->second);
			}
		}
	}
	
private:
	Type type_;
	variable_t var_;
	constant_t cons_;
	std::string name_;
	std::vector<Value> values_;
};

std::ostream& operator<<(std::ostream& stream, const Value& val);

struct Atom {
	std::string name;
	std::vector<Value> args;
	
	std::vector<Attribute>::size_type arity() const { return args.size(); }
	
	std::unordered_set<variable_t> variables() const {
		std::unordered_set<variable_t> v;
		for (const Value &val : args) {
			if (val.type() == Value::Type::VARIABLE) {
				v.insert(val.asVariable());
			}
		}
		return v;
	}
	
	void skolemize(const substitution_t& substitution) {
		for (Value &val : args) {
			val.skolemize(substitution);
		}
	}
};

std::ostream& operator<<(std::ostream& stream, const Atom& atom);

using Query = std::vector<Atom>;

std::ostream& operator<<(std::ostream& stream, const Query& query);

struct Tgd {
	struct FreeBoundVarPartition {
		std::unordered_set<variable_t> free;
		std::unordered_set<variable_t> bound;
	};
	
	Query from;
	Query to;
	
	std::unordered_set<variable_t> freeVariables () const {
		std::unordered_set<variable_t> fv, from_v;
		for (const Atom& at : from) {
			for (variable_t var : at.variables()) {
				from_v.insert(std::move(var));
			} 
		}
		for (const Atom& at : to) {
			for (const variable_t &var : at.variables()) {
				if (from_v.count(var) == 0) {
					fv.insert(var);
				}
			} 
		}
		return fv;
	}
	
	std::unordered_set<variable_t> boundVariables () const {
		std::unordered_set<variable_t> bv, from_v;
		for (const Atom& at : from) {
			for (variable_t var : at.variables()) {
				from_v.insert(std::move(var));
			} 
		}
		for (const Atom& at : to) {
			for (const variable_t &var : at.variables()) {
				if (from_v.count(var) != 0) {
					bv.insert(var);
				}
			} 
		}
		return bv;
	}
	
	FreeBoundVarPartition freeBoundVariables () const {
		std::unordered_set<variable_t> from_v;
		FreeBoundVarPartition fbvp;
		for (const Atom& at : from) {
			for (variable_t var : at.variables()) {
				from_v.insert(std::move(var));
			} 
		}
		for (const Atom& at : to) {
			for (const variable_t &var : at.variables()) {
				if (from_v.count(var) == 0) {
					fbvp.free.insert(var);
				} else {
					fbvp.bound.insert(var);
				}
			} 
		}
		return fbvp;
	}
	
	void skolemize () {
		FreeBoundVarPartition fbvp = freeBoundVariables();
		substitution_t substitution;
		for (const variable_t &var : fbvp.free) {
			substitution.insert({var, Value{std::string("sk") + std::to_string(current_skolem_++), var, {fbvp.bound.begin(), fbvp.bound.end()}}});
		}
		for (Atom &at : to) {
			at.skolemize(substitution);
		}
	}
private:
	static unsigned long long current_skolem_;
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