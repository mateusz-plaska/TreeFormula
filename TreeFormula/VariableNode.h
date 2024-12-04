#pragma once
#include "Node.h"

#include<map>
#include <string>

class VariableNode : public Node {
public:
	VariableNode(const std::string& name) : name(name) {}

	double evaluate(const std::map<std::string, double>& variables) const override {
		return variables.at(name);
	}

	std::string getSymbol() const override {
		return name;
	}

	bool isOperatorNode() const override {
		return false;
	}

private:
	std::string name;
};

