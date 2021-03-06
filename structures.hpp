#pragma once


#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <cassert>

#include "utils.hpp"


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

using Schema = std::unordered_map<std::string, Relation>;

std::ostream& operator<<(std::ostream& stream, const Schema& sc);


struct Value {
	enum class Type {VARIABLE, CONSTANT, SKOLEM, NO_TYPE};
	struct SkolemReturnType {
		const std::string& name;
		const variable_t& variable;
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
	
	struct Bounder {
		Bounder(std::string relation, size_t ident) : rel{relation}, id{ident} {}
		std::string rel;
		size_t id;
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
				if (from_v.find(var) == from_v.end()) {
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
			substitution.insert({var, Value{std::to_string(current_skolem_++), var, {fbvp.bound.begin(), fbvp.bound.end()}}});
		}
		for (Atom &at : to) {
			at.skolemize(substitution);
		}
	}
	
	std::unordered_multimap<variable_t, Bounder> varBounders() const {
		std::unordered_multimap<variable_t, Bounder> bounders;
		for (const Atom& at : from) {
			for (size_t i=0; i<at.args.size(); ++i) {
				if (at.args.at(i).type() == Value::Type::VARIABLE)
					bounders.insert({at.args.at(i).asVariable(), Bounder(at.name, i)});
			} 
		}
		return bounders;
	}
	
	std::string toSqlStatement (const Schema& fromSchema, const Schema& toSchema) const {
		std::ostringstream stream;
		
		auto varBnds = varBounders();
		
		for (const auto &atom : to) {
			const Relation& targetRelation = toSchema.at(atom.name);
			stream << "INSERT INTO" << std::endl
			       << tabs() << targetRelation << std::endl
			       << "SELECT" << std::endl;
			bool first{true};
			size_t argId=0;
			for (const auto &val : atom.args) {
				if (first) {
					first = false;
				} else {
					stream << "," << std::endl;
				}
				stream << tabs();
				if (val.type() == Value::Type::VARIABLE) {
					auto it = varBnds.find(val.asVariable());
					if (it == varBnds.end()) {
						throw std::runtime_error("Variable is not bounded...");
					}
					const Relation& rel(fromSchema.at(it->second.rel));
					stream << rel.name << '.' << rel.attributes.at(it->second.id);
				} else if (val.type() == Value::Type::CONSTANT) {
					stream << val.asConstant();
				} else if (val.type() == Value::Type::SKOLEM) {
					auto skolem = val.asSkolem();
					
					stream << "'sk[" << skolem.name << ","<< targetRelation.name << '.' << targetRelation.attributes.at(argId) << "]('";
					bool first2{true};
					for (const auto& var : skolem.values) {
						if (first2) {
							first2 = false;
						} else {
							stream << " || ','";
						}
						if (var.type() != Value::Type::VARIABLE) {
							throw std::logic_error("Logic error: Skolem subvalue is not a variable...");
						}
						auto it = varBnds.find(var.asVariable());
						if (it == varBnds.end()) {
							throw std::runtime_error("Skolem subvariable is not bounded...");
						}
						const Relation& rel(fromSchema.at(it->second.rel));
						stream << " || " << rel.name << '.' << rel.attributes.at(it->second.id);
					}
					stream << " || ')'";
				} else {
					throw std::logic_error("Logic error while computing SQL: Value of type NONE...");
				}
				++argId;
			}
			
			stream << std::endl << "FROM" << std::endl;
			
			std::unordered_set<std::string> done;
			
			first = true;
			for (const Atom& fromAtom : from) {
				stream << tabs();
				if (!first) {
					stream << "INNER JOIN ";
				}
				stream << fromAtom.name << std::endl;
				if (first) {
					first = false;
					done.insert(fromAtom.name);
					continue;
				}
				bool first2{true};
				argId = 0;
				for (const Value& arg : fromAtom.args) {
					if (arg.type() != Value::Type::VARIABLE) {
						++argId;
						continue;
					}
					auto range = varBnds.equal_range(arg.asVariable());
					if (range.first == varBnds.end()) {
						throw std::logic_error("Logic error while computing SQL: Variable is not bound...");
					}
					for (auto it = range.first; it != range.second; ++it) {
						const std::string& otherRelation = it->second.rel;
						size_t otherId = it->second.id;
						if (done.count(otherRelation) > 0) {
							if (first2) {
								stream << tabs() << "ON (" << std::endl << tabs(2);
								first2 = false;
							} else {
								stream << tabs(2) << "AND ";
							}
							stream << fromAtom.name << "." << fromSchema.at(fromAtom.name).attributes.at(argId) << " = " << otherRelation << "." << fromSchema.at(otherRelation).attributes.at(otherId) << std::endl;
							break;
						}
					}
					++argId;
				}
				if (!first2) { // Found something, "ON (" is open, must close with ")"
					stream << tabs() << ")" << std::endl;
				}
				done.insert(fromAtom.name);
			}
			
			stream << ";" << std::endl;
		}
		
		return stream.str();
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
