// File: MultiThreadScanner.hpp
// Date: Nov, 1, 2021
// Student: Sion Young
// Desc: A utility that scans a file with multi-thread approach

#pragma once
#include "Report.hpp"
#include "UserCommand.hpp"
#include <latch>

class MultiThreadScanner {

public:
	virtual ~MultiThreadScanner() = default;

	virtual Report scan() = 0;
};

