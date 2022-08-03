// File: ReportItem.hpp
// Date: Nov, 2, 2021
// Student: Sion Young
// Desc: A data class that represents a single element of the final report

#pragma once

#include <set>
#include <string>
#include <format>
#include <iostream>

class ReportItem {
private:
	std::string content ="";
	size_t lineNumber = 0 ;
	size_t numInstances = 0;

public:

	std::string getContent() { return content; }
	void setContent(std::string const& c ) {  content = c; }
	unsigned getLineNumber() { return lineNumber; };
	void setLineNumber(size_t const& n) { lineNumber = n; };
	unsigned getNumInstances() { return numInstances; };
	void setNumInstances(size_t const& n) { numInstances = n; };

	void printItem() {
		std::cout << std::format("[{}{}] {}\n", lineNumber, numInstances == 1? "" : ":" + std::to_string(numInstances), content);
	}

	bool operator < (ReportItem const& item) const {
		return lineNumber < item.lineNumber;
	}
};
