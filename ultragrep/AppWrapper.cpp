// File: AppWrapper.cpp
// Date: Oct, 4, 2021
// Student: Sion Young
// Desc: a wrapper class to launch an app in singleton pattern and handle exception globally

#include "ConsoleApp.hpp"
#include <fstream>
#include <cstdlib>
#include <crtdbg.h>
#include <iostream>

using namespace std;

ConsoleApp* ConsoleApp::thisApp_sm = nullptr;

ConsoleApp::ConsoleApp()
{
	if (thisApp_sm)
	{
		cerr << "Error: more than one instance of the App";
		exit(EXIT_FAILURE);
	}
	thisApp_sm = this;
}

int main(int argc, char* argv[]) try {
#if defined(_DEBUG) && defined(_WIN32)
	int dbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	dbgFlags |= _CRTDBG_CHECK_ALWAYS_DF;
	dbgFlags |= _CRTDBG_DELAY_FREE_MEM_DF | dbgFlags;
	dbgFlags |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(dbgFlags);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
#endif

	std::set_terminate([]()
		{
			cout << "Error (main): Unhandled exception\n";
			exit(EXIT_FAILURE);
		});

	ConsoleApp::thisApp_sm->args_m.assign(argv, argv + argc);
	return ConsoleApp::thisApp_sm->execute();
}
catch (string const& msg) {
	cerr << "Error: " << msg << endl;
	return EXIT_FAILURE;
}
catch (std::logic_error const& e) {
	cerr << "Error" << e.what() << endl;
	return EXIT_FAILURE;
}
catch (std::runtime_error const& e) {
	cerr << "Error: " << e.what() << endl;
	return EXIT_FAILURE;
}
catch (std::exception const& e) {
	cerr << "Error: " <<e.what() << std::endl;
	return EXIT_FAILURE;
}