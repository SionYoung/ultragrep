// File: ultragrep.cpp 
// Date: November 1, 2021
// Student: Sion Young
// Desc: This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>

#include "Report.hpp"
#include "Scanner.hpp"
#include "ConsoleApp.hpp"
#include "UserCommand.hpp"
#include <chrono>
#include "WIN32Scanner.hpp"
#include "ScannerAdaptor.hpp"

using namespace std;

MAKEAPP(Ultragrep) {

	auto start = chrono::steady_clock::now();

	auto commands = UserCommand::parseCommand(get_args().size(), get_args().data());

	unique_ptr<Scanner> scanner = make_unique<ScannerAdaptor>(commands);

	Report report = scanner->scan();

	auto end = chrono::steady_clock::now();

	report.setTimeElapsed(chrono::duration_cast<chrono::nanoseconds>(end - start).count() / 1000000000.0);
	report.printReport();

	return EXIT_SUCCESS;
}