// File: CPP20Scanner.cpp
// Date: Nov, 2, 2021
// Student: Sion Young
// Desc: Implimentation of CPP_20_Scanner interface

#pragma once
#include "Scanner.hpp"
#include "CPP20Scanner.hpp"
#include "Report.hpp" 

#include <regex>
#include <mutex>
#include <vector>
#include <atomic>
#include <thread>
#include <barrier>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <condition_variable>

using namespace std;

Report CPP20Scanner::scan() {

	vector<jthread> threads;
	auto extList = command.getExtensionList();

	for (size_t i = 0; i < nThreads; ++i) {
		threads.push_back(jthread(&CPP20Scanner::grep, this));
	}

	for (const auto& entry : filesystem::recursive_directory_iterator(command.getFolder())) {
		if (isVerbose && entry.is_directory()) {
			{
				lock_guard<mutex>lk(consoleMtx);
				cout << "\nScanning: " << entry.path().string() << endl;
			}
		}
		if (extList.contains(entry.path().extension().string())) {
			{
				lock_guard<mutex>lk(taskMtx);
				tasks.push(entry.path().string());
			}
			wakeCond.notify_one();
		}
	}

	morePossibleWork = false;
	wakeCond.notify_all();

	for (auto & t : threads) 
		t.join();

	report.setTotalMatches(totalMatches);
	return report;
}

void CPP20Scanner::grep() {

	setupDone.count_down();

	while (morePossibleWork && tasks.empty()) {
		{ unique_lock<mutex> lk(wakeMtx); wakeCond.wait(lk); }

		while (!tasks.empty()) {
			string task;
			bool haveTask = false;
			fstream file;
			{
				lock_guard<mutex> lk(taskMtx);
				if (!tasks.empty()) {
					task = tasks.front();
					tasks.pop();
					file.open(task);
					haveTask = true;
				}
			}
			if (haveTask) {
				if (isVerbose) {
					{
						lock_guard<mutex> lk(consoleMtx);
						std::cout << "Grepping: " << task << endl;
					}
				}

				string buf;
				size_t lineNum = 0;
				unsigned instances = 0;
				bool isMatch = false;
				while (getline(file, buf)) {
					std::regex_token_iterator<std::string::iterator> rend;
					std::regex_token_iterator<std::string::iterator> a(buf.begin(), buf.end(), reg);
					++lineNum;

					if (a == rend) continue; // the current line has has no match

					while (a != rend) {
						++a;
						++instances;
					}

					if (isVerbose) {
						{
							lock_guard<mutex> lk(consoleMtx);
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
						lock_guard<mutex> lk(totalMatchesMtx);
						totalMatches += instances;
					}
					instances = 0;
				}

				if (isMatch) {
					{
						lock_guard<mutex> lk(fileMatchesMtx);
						report.incrementFileWithMatches();
					}
				}

				file.close();
			}
		}
	}

}
