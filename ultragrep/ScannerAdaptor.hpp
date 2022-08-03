// File: ScannerAdaptor.hpp
// Date: Nov, 3, 2021
// Student: Sion Young
// Desc: A utitily that hides implementation of the scanning and grepping logics

#pragma once

#include "Scanner.hpp"
#include "WIN32Scanner.hpp"
#include "CPP20Scanner.hpp"
#include "MultiThreadScanner.hpp"
#include "SingleThreadScanner.hpp"


#include <filesystem>
#include <Windows.h>
#include <latch>
#include <thread>

namespace fs = std::filesystem;


class ScannerAdaptor : public Scanner {
private:
	std::unique_ptr<MultiThreadScanner> scanner;
	//std::unique_ptr<SingleThreadScanner> scanner;
public:
	ScannerAdaptor() = delete;
	ScannerAdaptor(UserCommand const& commands) {
		if (!fs::is_directory(commands.getFolder()))
			throw std::string("not a folder");

		scanner = std::make_unique<CPP20Scanner>(commands);
		//scanner = std::make_unique<WIN32Scanner>(commands);
		//scanner = std::make_unique<SingleThreadScanner>(commands);
	}

	Report scan() override {
		return scanner->scan();
	}
};