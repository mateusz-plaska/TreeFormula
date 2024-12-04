#pragma once
#include "Node.h"

#include<map>
#include<string>

class NumberNode : public Node {
public:
	NumberNode(unsigned long long value) : value(value) {}

	double evaluate(const std::map<std::string, double>& variables) const override {
		return value;
	}

	std::string getSymbol() const override {
		return std::to_string(value);
	}

	bool isOperatorNode() const override {
		return false;
	}

private:
	unsigned long long value;
};

