// File: CPP_20_Scanner.hpp
// Date: Nov, 2, 2021
// Student: Sion Young
// Desc: A specification of MultiThreadScanner that uses c++ 20 library for threading 

#pragma once
#include "MultiThreadScanner.hpp"

#include <queue>
#include <string>
#include <atomic>
#include <latch>
#include <string>
#include <mutex>

class CPP20Scanner : public MultiThreadScanner {

	Report report;
	UserCommand command;
	std::size_t totalMatches = 0;
	std::size_t fileWithMatches = 0;

	bool morePossibleWork = true;
	bool isVerbose = false;

	std::size_t nThreads = std::thread::hardware_concurrency();

	std::regex reg;
	std::latch setupDone{ nThreads + 1 };
	std::queue<std::string> tasks;

	// wake
	std::mutex wakeMtx;
	std::condition_variable wakeCond;

	// queue of tasks to perform
	std::mutex taskMtx;
	std::mutex totalMatchesMtx;
	std::mutex fileMatchesMtx;
	std::mutex consoleMtx;
	void grep();

public:
	CPP20Scanner() = delete;
	CPP20Scanner(UserCommand const& c){
		command = c;
		isVerbose = command.getIsVerbose();
		reg = std::regex(command.getExpression());
	}
	virtual Report scan() override;
};

