// File: CommonUtils.hpp
// Date: Oct, 7, 2021
// Student: Sion Young
// Desc: a class for implementing singleton application pattern 

#pragma once
#if defined(_WIN32)
#include <crtdbg.h>
#include <Windows.h>
#endif
#include <vector>
#include <string>


class ConsoleApp
{
	friend int main(int argc, char* argv[]);
	static ConsoleApp* thisApp_sm;
	using CommandLineArgs = std::vector<std::string>;
	CommandLineArgs args_m;
public:
	ConsoleApp();
	virtual int execute() = 0;
	CommandLineArgs const& get_args() const { return args_m; }
};

#define MAKEAPP(name)\
	class name : public ConsoleApp{\
		int execute() override;\
} name##_g;\
int name ::execute()

