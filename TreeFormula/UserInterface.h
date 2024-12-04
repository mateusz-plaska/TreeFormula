#pragma once

#include "Tree.h"

#include <string>
#include <vector>
#include <map>

namespace CommandNames {
	const std::string ENTER = "enter";
	const std::string VARS = "vars";
	const std::string PRINT = "print";
	const std::string COMP = "comp";
	const std::string JOIN = "join";
	const std::string EXIT = "exit";
	const std::string TREE_ROWS = "print_rows";
}

class UserInterface {
	using Commands = std::map<const std::string, void (UserInterface::*)(const std::string&)>;

public:
	void run();

private:
	Tree tree;
	const static Commands commandMap;

	const static Commands& getCommandsMap();
	void handleEnter(const std::string& args);
	void handleVars(const std::string&);
	void handlePrint(const std::string&);
	void handleComp(const std::string& args);
	void handleJoin(const std::string& args);
	void handleTreeRows(const std::string& args);

	void comp(const std::vector<double>& values) const;
};

