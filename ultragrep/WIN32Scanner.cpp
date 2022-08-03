// File: WIN32Scanner.cpp
// Date: Nov, 6, 2021
// Student: Sion Young
// Desc: Implimentation of WIN_32_Scanner interface

#include "Report.hpp" 
#include "WIN32Scanner.hpp"
#include "CriticalSection.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <windows.h>
#include <string>

using namespace std;
namespace fs = std::filesystem;

bool areThreadsReady = false;
queue<string> tasks;
UserCommand command;
Report report;
std::size_t totalMatches = 0;
std::size_t fileWithMatches = 0;

unsigned getNumThreads() {
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	return si.dwNumberOfProcessors;
}

std::size_t nThreads = getNumThreads();
unsigned barrierThreshold = getNumThreads() + 1;
unsigned barrierCount = barrierThreshold;

bool morePossibleWork = true;
CriticalSection taskMtx;
CriticalSection consoleMtx;
CriticalSection fileMatchesMtx;

HANDLE barrierEvent = NULL;
HANDLE wakeEvent = NULL;

void barrier() {
	//auto gen = barrierGeneration;
	if (--barrierCount == 0) {
		areThreadsReady = true;
		SetEvent(barrierEvent);
		{ CSLock lk(consoleMtx);
		cout << '[' << GetCurrentThreadId() << "]\tBarrier collapsed." << endl; }
	}
	else {
		{ CSLock lk(consoleMtx);
		cout << '[' << GetCurrentThreadId() << "]\tBarrier hit." << endl; }
		while (!areThreadsReady)
			WaitForSingleObject(barrierEvent, INFINITE); }
}

DWORD WINAPI WIN32Scanner::grep(LPVOID) {

	barrier();

	while (morePossibleWork) {
		WaitForSingleObject(wakeEvent, INFINITE); // wait for specified object is in signaled state

		while (!tasks.empty()) {
			string task;
			bool haveTask = false;
			fstream file;
			{
				CSLock lk(taskMtx);
				if (!tasks.empty()) {
					task = tasks.front();
					tasks.pop();
					file.open(task);
					haveTask = true;
				}
			}
			if (haveTask) {
				if (command.getIsVerbose()) {
					{
						CSLock lk(consoleMtx);
						std::cout << "Grepping: " << task << endl;
					}
				}

				string buf;
				size_t lineNum = 0;
				unsigned instances = 0;
				bool isMatch = false;
				regex rr(command.getExpression());

				while (getline(file, buf)) {
					auto begin = sregex_iterator(buf.begin(), buf.end(), rr);
					auto end = sregex_iterator();
					instances = distance(begin, end);
					if (instances == 0) continue;

					++lineNum;

					if (command.getIsVerbose()) {
						{
							CSLock lk(consoleMtx);
							cout << "Matched: " << instances << " [" << lineNum << ']' << buf << endl;
						}
					}

					isMatch = instances > 0 || isMatch;

					ReportItem item;
					item.setContent(buf);
					item.setLineNumber(lineNum);
					item.setNumInstances(instances);
					report.addReportItem(task, item);
					{
						CSLock lk(consoleMtx);
						totalMatches += instances;
					}
					instances = 0;
				}

				if (isMatch) {
					{
						CSLock lk(fileMatchesMtx);
						report.incrementFileWithMatches();
					}
				}

				file.close();
			}
		}
	}

	return 0;
}

Report WIN32Scanner::scan() {
	if ((wakeEvent = CreateEvent(NULL, FALSE, FALSE, NULL)) == NULL)
		throw string("Unexpected Error");

	barrierEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	vector<HANDLE> threads;
	for (unsigned i = 0; i < nThreads; ++i) {
		threads.push_back(CreateThread(NULL, 0, grep, NULL, 0, NULL));
	}

	barrier();

	command = _command;
	bool isVerbose = _command.getIsVerbose();
	set<string> extList = _command.getExtensionList();

	for (const auto& entry : filesystem::recursive_directory_iterator(_command.getFolder())) {
		if (isVerbose && entry.is_directory()) {
			{
				CSLock lk(consoleMtx);
				cout << "\nScanning: " << entry.path().string() << endl;
			}
		}

		if (extList.contains(entry.path().extension().string())) {
			{
				CSLock lk(taskMtx);
				tasks.push(entry.path().string());
			}
			(void)SetEvent(wakeEvent);
		}
	}

	morePossibleWork = false;
	for (size_t i = 0; i < nThreads; ++i)
		SetEvent(wakeEvent);

	// cleanup
	WaitForMultipleObjects((DWORD)threads.size(), threads.data(), TRUE, INFINITE);
	for (auto& t : threads)
		CloseHandle(t);
	if (barrierEvent != 0) {
		(void)CloseHandle(barrierEvent);
	}
	(void)CloseHandle(wakeEvent);

	report.setTotalMatches(totalMatches);
	return report;
}
