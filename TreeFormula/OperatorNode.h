#pragma once
#include "Node.h"
#include "NumberNode.h"
#include "VariableNode.h"

#include <vector>
#include <string>
#include <functional>
#include <map>
#include <sstream>


class OperatorNode : public Node {
public:
	OperatorNode(const std::string operatorSymbol, const int NUMBER_OF_ARGUMENTS) : 
		operatorSymbol(operatorSymbol), NUMBER_OF_ARGUMENTS(NUMBER_OF_ARGUMENTS) {}

	~OperatorNode() override {
		for (Node* child : children) {
			if (!dynamic_cast<VariableNode*>(child)) {
				delete child;
			}
		}
	}

	virtual Node* getDefaultChild() const {
		return new NumberNode(1);
	}

	void addChild(Node* child) {
		children.push_back(child);
	}

	std::string getSymbol() const override {
		return operatorSymbol;
	}

	std::vector<Node*>& getChildren() {
		return children;
	}

	std::string toString() const override {
		std::ostringstream result;
		result << Node::toString();

		for (const auto& child : children) {
			result << child->toString();
		}
		return result.str();
	}

	const int getNumberOfArguments() const {
		return NUMBER_OF_ARGUMENTS;
	}

	bool isOperatorNode() const override {
		return true;
	}

protected:
	std::string operatorSymbol;
	std::vector<Node*> children;
	const int NUMBER_OF_ARGUMENTS;
};

