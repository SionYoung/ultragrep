// File: WIN32Scanner.hpp
// Date: Nov, 5, 2021
// Student: Sion Young
// Desc: A specification of MultiThreadScanner that uses win 32 library for threading 

#pragma once
#include "MultiThreadScanner.hpp"
#include "CriticalSection.hpp"
#include <Windows.h>
#include <queue>
#include <atomic>


class WIN32Scanner : public MultiThreadScanner {

	static DWORD WINAPI grep(LPVOID);
public:

	UserCommand _command;
	WIN32Scanner() = delete;
	WIN32Scanner(UserCommand const& c) {
		_command = c;
	}

	virtual Report scan() override;
};

