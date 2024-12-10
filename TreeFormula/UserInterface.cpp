#include "UserInterface.h"
#include "Messages.h"
#include "Error.h"
#include "Result.h"


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
	Result<Tree*, Error> result = this->tree.createTree(args);

	if (!result.isSuccess()) {
		for (Error* error : result.getErrors()) {
			cout << error->getErrorDescription() << endl;
		}

		cout << endl << Messages::formatMessage(Messages::FINAL_FORMULA,
			{ {Messages::toReplaceText::formula, this->tree.toString()} }) << endl;
	}
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
		double value;
		try {
			value = stod(arg);
		}
		catch (const std::exception& e) {
			cout << endl << Messages::formatMessage(Messages::ERROR_INVALID_VALUE, 
				{ {Messages::toReplaceText::valueStr, arg} }) << endl;
			return;
		}
		values.push_back(value);
	}

	this->comp(values);
}

void UserInterface::handleJoin(const string& args) {
	Result<Tree*, Error> result = this->tree.join(args);

	if (!result.isSuccess()) {
		for (Error* error : result.getErrors()) {
			cout << error->getErrorDescription() << endl;
		}

		cout << endl << Messages::formatMessage(Messages::FINAL_FORMULA,
			{ {Messages::toReplaceText::formula, this->tree.toString()} }) << endl;
	}
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

	Result<double, Error> computeResult = this->tree.computeValue(variablesValues);

	if (!computeResult.isSuccess()) {
		for (Error* error : computeResult.getErrors()) {
			cout << error->getErrorDescription() << endl;
		}
		return;
	}
	cout << computeResult.getValue() << endl;
}