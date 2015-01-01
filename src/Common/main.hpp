#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>

struct options
{
	std::string clientName;
};

options processArguments(int argc, char* argv[]);

#endif // MAIN_HPP
