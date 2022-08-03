// File: Scanner.hpp
// Date: Nov, 1, 2021
// Student: Sion Young
// Desc: An interface that represents a Scanner for client to scan through a folder

#pragma once

#include "UserCommand.hpp"
#include "Report.hpp"


class Scanner {

public:
	virtual ~Scanner() = default;

	virtual Report scan() = 0;
};

