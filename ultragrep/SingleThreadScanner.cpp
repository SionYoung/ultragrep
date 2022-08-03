// File: SingleThreadScanner.cpp
// Date: Oct, 30, 2021
// Student: Sion Young
// Desc: Implimentation of SingleThreadScanner interface

#include "SingleThreadScanner.hpp"
#include "Report.hpp" 
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <string>
#include <atomic>

using namespace std;
namespace fs = std::filesystem;

Report SingleThreadScanner::scan() {

	Report report;
	auto extList = command.getExtensionList();

	for (const auto& entry : fs::recursive_directory_iterator(command.getFolder())) {

		if (isVerbose&& entry.is_directory())
			std::cout << "\nScanning: " << entry.path() << endl;

		auto ext = entry.path().extension().string();

		if (!fs::is_regular_file(entry.path()) || !extList.contains(ext)) continue;


		if (isVerbose) std::cout << "Grepping: " << entry.path() << endl;

		grep(entry.path().string());
	}

	report.setFileWithMatches(fileWithMatches);
	report.setTotalMatches(totalMatches);
	return report;
}

void SingleThreadScanner::grep(string const& path) {

	string buf;
	size_t lineNum = 0;
	regex reg(command.getExpression());

	bool isMatch = false;
	fstream file; 
	file.open(path);

	while (getline(file, buf) && ++lineNum) {
		unsigned instances = 0;
		std::regex_token_iterator<std::string::iterator> rend;
		std::regex_token_iterator<std::string::iterator> a(buf.begin(), buf.end(), reg);

		if (a == rend) continue; // the current line has has no match

		while (a != rend) {
			++a;
			++instances;
		}
		isMatch = instances > 0 || isMatch;
		totalMatches += instances;

		if (isVerbose) {
			cout << "Matched: " << instances << '\" ' << path
				<< '\"' << " [" << lineNum << ']' << buf << endl;
		}

		ReportItem item;
		item.setContent(buf);
		item.setLineNumber(lineNum);
		item.setNumInstances(instances);

		report.addReportItem(path, item);

	}
	fileWithMatches += isMatch;
	file.close();
}