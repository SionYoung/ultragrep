// File: UserCommand.hpp
// Date: Nov, 1, 2021
// Student: Sion Young
// Desc: A data class that stores user commands

#pragma once

#include <set>
#include <regex>
#include <string>
#include <iostream>

class UserCommand {
private:
	bool isVerbose = false;
	std::string folder;
	std::regex expression;
	std::set<std::string> extensionList;

	static std::set<std::string> parseUserExtension(std::string const* userExtensions);
public:
	// getters and setters
	bool getIsVerbose() const { return isVerbose; };
	void setIsVerbose(bool const& v) { isVerbose = v; };
	std::string getFolder() const { return folder; };
	void setFolder(std::string const& f) { folder = f; };
	std::regex getExpression () const { return expression; }
	void setExpression(std::regex const& exp) { expression = exp; }
	std::set<std::string> getExtensionList () const { return extensionList; }
	void setExtensionList(std::set<std::string> const& list) { extensionList = list; };

	// utils
	static UserCommand parseCommand(size_t const& numArgs, std::string const* userCommands);
};