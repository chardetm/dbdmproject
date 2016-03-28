#include "utils.hpp"

#include <string>


std::string spaces(unsigned n, char c) {
	return std::string(n, c);
}
std::string tabs(unsigned n) {
	return std::string(n, '\t');
}
