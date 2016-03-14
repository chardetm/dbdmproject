#include "structures.hpp"

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

std::ostream& operator<<(std::ostream& stream, const Schema& sc) {
	for (const auto& rel : sc) {
		stream << rel << std::endl;
	}
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Value& val) {
	if (val.type() == Value::Type::CONSTANT) {
		stream << val.asConstant();
	} else if (val.type() == Value::Type::VARIABLE) {
		stream << '$' << val.asVariable();
	} else {
		stream << "{no value}";
	}
	return stream;
}

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

std::ostream& operator<<(std::ostream& stream, const Tgd& tgd) {
	stream << tgd.from << "  ->  " << tgd.to;
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const Mapping& mapping) {
	for (const auto& tgd : mapping) {
		stream << tgd << std::endl;
	}
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const FileContent& fc) {
	stream << "Source:" << std::endl << fc.source << std::endl << std::endl << "Target:" << std::endl << fc.target << std::endl << std::endl << "Mapping:" << fc.mapping;
	return stream;
}
