// File: Report.hpp
// Date: Nov, 2, 2021
// Student: Sion Young
// Desc: A data class that represents the final report

#pragma once

#include "ReportItem.hpp"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <format>
#include <iostream>

class Report {
	std::map<std::string, std::set<ReportItem>> report;
	size_t totalMatches = 0;
	size_t fileWithMatches = 0;
	double timeElapsed = 0;

public:
	void addReportItem(std::string const& path, ReportItem const& r){ report[path].insert(r); }
	void setTotalMatches(size_t const& m) { totalMatches = m; }
	void incrementFileWithMatches() { ++fileWithMatches; }
	void setFileWithMatches(unsigned const& m) { fileWithMatches = m; }
	void setTimeElapsed(double const& t) { timeElapsed = t; };

	void printReport() {
		std::cout << "Grep report\n" << std::endl;
		for (auto const& entry : report) {
			std::cout << '\"' << entry.first <<'\"' << std::endl;
			for (auto s : entry.second) {
				s.printItem();
			}
			std::cout << std::endl;
		}
		std::cout.imbue(std::locale("")); 
		std::cout << "File with matches = " << std::fixed << fileWithMatches << std::endl;
		std::cout << "Total number with matches = " << totalMatches << std::endl;
		std::cout << "Scan completed in " << timeElapsed <<  's' << std::endl;
	}
};
