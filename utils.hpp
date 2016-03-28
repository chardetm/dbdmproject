#pragma once


#include <string>
#include <iostream>


std::string spaces(unsigned n=1, char c=' ');
std::string tabs(unsigned n=1);


template <class SetType>
std::ostream& printSet(std::ostream& stream, const SetType& set, std::string prefix="", std::string suffix="") {
	bool first{true};
	for (const auto &v : set) {
		if (first) {
			first = false;
		} else {
			stream << ", ";
		}
		stream << prefix << v << suffix;
	}
	return stream;
}