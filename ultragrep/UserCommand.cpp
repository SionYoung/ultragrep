// File: UserCommand.cpp
// Date: Nov, 1, 2021
// Student: Sion Young
// Desc: Implementation of UserCommand member function

#include "UserCommand.hpp" 
#include <sstream>

using namespace std;

UserCommand UserCommand::parseCommand(size_t const& numArgs, string const* userCommands) {

	if (numArgs == 1) 
		throw string("ultragrep 1.0.0 (c) 2021, Sion Young\nUsage: ultragrep [-v] folder regex [.ext]*");

	if (numArgs == 2 || (*(userCommands + 1) == "-v" && numArgs == 3)) 
		throw string("Error: No regular expression");

	UserCommand cmd {};
	unsigned idx = 1; 

	if (*(userCommands + idx) == "-v") { 
		cmd.setIsVerbose(true);
		idx++;
	}

	cmd.setFolder(*(userCommands + idx++)); 

	cmd.setExpression(regex(*(userCommands + idx++)));

	if (idx == numArgs) {
		set<string> extension{ ".txt" };
		cmd.setExtensionList(extension);
	}
	else {
		cmd.setExtensionList(parseUserExtension(userCommands + idx));
	}

	return cmd;
}

set<string> UserCommand::parseUserExtension(string const* userExtensions) {

	set<string> extensions;
	std::stringstream ss(*userExtensions);

	while (ss.good()) {
		string buf;
		getline(ss, buf, '.');
		if(!buf.empty())
			extensions.insert('.' + buf);
	}
	return extensions;
}
