// File: SingleThreadScanner.hpp
// Date: Oct, 28, 2021
// Student: Sion Young
// Desc: Single threaded scanner

#pragma once
#include "Scanner.hpp"
#include <string>

class SingleThreadScanner  {

	Report report;
	UserCommand command;

	unsigned fileWithMatches = 0;
	unsigned totalMatches = 0;
	bool morePossibleWork = true;
	bool isVerbose = false;

	void grep(std::string const& path);
public:
	SingleThreadScanner(UserCommand const& c) {
		command = c;
		isVerbose = command.getIsVerbose();
	}
	Report scan();
};

