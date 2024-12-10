#pragma once
#include "Node.h"
#include "Result.h"
#include "Error.h"
#include "Messages.h"

#include<map>
#include <string>

class VariableNode : public Node {
public:
	VariableNode(const std::string& name) : name(name) {}

	Result<double, Error> evaluate(const std::map<std::string, double>& variables) const override {
		auto it = variables.find(name);
		if (it == variables.end()) {
			return Result<double, Error>::fail(Error(Messages::formatMessage(
				Messages::ERROR_MISSING_VALUE_FOR_VAR, { {Messages::toReplaceText::name, name} })));
		}
		return Result<double, Error>::ok(it->second);
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

