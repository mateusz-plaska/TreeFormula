#include "UserInterface.h"
#include "Messages.h"


#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <map>
using namespace std;


const UserInterface::Commands UserInterface::commandMap = UserInterface::getCommandsMap();

const UserInterface::Commands& UserInterface::getCommandsMap() {
	static Commands commands;

	commands[CommandNames::ENTER] = &UserInterface::handleEnter;
	commands[CommandNames::VARS] = &UserInterface::handleVars;
	commands[CommandNames::PRINT] = &UserInterface::handlePrint;
	commands[CommandNames::COMP] = &UserInterface::handleComp;
	commands[CommandNames::JOIN] = &UserInterface::handleJoin;
	commands[CommandNames::TREE_ROWS] = &UserInterface::handleTreeRows;

	return commands;
}


void UserInterface::run() {
	string input;

	string commands = Messages::SPACE_MSG;
	for (const auto& pair : commandMap) {
		commands += pair.first + Messages::SPACE_MSG;
	}
	commands += CommandNames::EXIT + Messages::SPACE_MSG;

	while (true) {
		cout << endl << Messages::formatMessage(Messages::COMMAND_PROMPT, 
			{ {Messages::toReplaceText::commands, commands} }) << endl;

		getline(cin, input);

		istringstream prompt(input);
		string command, args;
		prompt >> command;
		
		if (command == CommandNames::EXIT) {
			cout << endl << Messages::PROGRAM_EXIT << endl;
			return;
		}

		getline(prompt, args);
		if (!args.empty()) {
			args = args.substr(1);
		}

		if (commandMap.find(command) != commandMap.end()) {
			(this->*commandMap.at(command))(args);
		}
		else {
			cout << endl << Messages::formatMessage(Messages::UNKNOWN_COMMAND, 
				{ {Messages::toReplaceText::command, command} }) << endl;
		}
	}
}

void UserInterface::handleEnter(const std::string& args) {
	string errors = this->tree.createTree(args);
	cout << endl << errors << endl;
}

void UserInterface::handleVars(const std::string&) {
	for (const string& var : this->tree.getVariableNames()) {
		cout << var << Messages::SPACE_MSG;
	}
	cout << endl;
}

void UserInterface::handlePrint(const std::string&) {
	cout << this->tree.toString() << endl;
}

void UserInterface::handleComp(const std::string& args) {
	istringstream argPrompt(args);
	string arg;
	vector<double> values;
	while ((argPrompt >> arg)) {
		values.push_back(stod(arg));
	}

	this->comp(values);
}

void UserInterface::handleJoin(const string& args) {
	string errors = this->tree.join(args);
	cout << endl << errors << endl;
}

void UserInterface::handleTreeRows(const string&) {
	int row = 1;
	for (vector<string>& a : this->tree.getTreeRows()) {
		cout << row << Messages::COLON_MSG + Messages::SPACE_MSG;
		for (string& e : a) {
			cout << e << Messages::SPACE_MSG;
		}
		cout << endl;
		row++;
	}
}


void UserInterface::comp(const vector<double>& values) const {
	const std::set<string>& variables = this->tree.getVariableNames();

	if (variables.size() != values.size()) {
		cout << endl << Messages::formatMessage(Messages::ERROR_COMP_VALUES, 
			{ {Messages::toReplaceText::expected, to_string(variables.size())}, 
			{Messages::toReplaceText::provided, to_string(values.size())}}) << endl;
		return;
	}

	map<string, double> variablesValues;

	int i = 0;
	for (const string& var : variables) {
		variablesValues[var] = values[i++];
	}

	cout << this->tree.computeValue(variablesValues) << endl;
}