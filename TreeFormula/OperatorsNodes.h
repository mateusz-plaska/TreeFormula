#pragma once
#include "OperatorNode.h"
#include "NumberNode.h"

#include <map>
#include <string>

const std::string ADD_OPERATOR = "+";
const std::string SUB_OPERATOR = "-";
const std::string MUL_OPERATOR = "*";
const std::string DIV_OPERATOR = "/";
const std::string SIN_OPERATOR = "sin";
const std::string COS_OPERATOR = "cos";

constexpr int ADD_ARGUMENTS = 2;
constexpr int SUB_ARGUMENTS = 2;
constexpr int MUL_ARGUMENTS = 2;
constexpr int DIV_ARGUMENTS = 2;
constexpr int SIN_ARGUMENTS = 1;
constexpr int COS_ARGUMENTS = 1;


class AdditionNode : public OperatorNode {
public:
    AdditionNode(const std::string operatorSymbol = ADD_OPERATOR) : OperatorNode(operatorSymbol, ADD_ARGUMENTS) {}

	double evaluate(const std::map<std::string, double>& variables) const override {
        double result = 0;
        for (const auto& child : children) {
            result += child->evaluate(variables);
        }
        return result;
	}
};

class SubtractionNode : public OperatorNode {
public:
    SubtractionNode(const std::string operatorSymbol = SUB_OPERATOR) : OperatorNode(operatorSymbol, SUB_ARGUMENTS) {}

    double evaluate(const std::map<std::string, double>& variables) const override {
        double result = 0;
        if (children.size()) {
            result = children.front()->evaluate(variables);
        }

        for (int i = 1; i < children.size(); ++i) {
            result -= children[i]->evaluate(variables);
        }
        return result;
    }
};

class MultiplicationNode : public OperatorNode {
public:
    MultiplicationNode(const std::string operatorSymbol = MUL_OPERATOR) : OperatorNode(operatorSymbol, MUL_ARGUMENTS) {}

    double evaluate(const std::map<std::string, double>& variables) const override {
        double result = 1;
        for (const auto& child : children) {
            result *= child->evaluate(variables);
        }
        return result;
    }
};

class DividingNode : public OperatorNode {
public:
    DividingNode(const std::string operatorSymbol = DIV_OPERATOR) : OperatorNode(operatorSymbol, DIV_ARGUMENTS) {}

    double evaluate(const std::map<std::string, double>& variables) const override {
        double result = 0;
        if (children.size()) {
            result = children.front()->evaluate(variables);
        }

        for (int i = 1; i < children.size(); ++i) {
            result /= children[i]->evaluate(variables);
        }
        return result;
    }
};

class SinNode : public OperatorNode {
public:
    SinNode(const std::string operatorSymbol = SIN_OPERATOR) : OperatorNode(operatorSymbol, SIN_ARGUMENTS) {}

    double evaluate(const std::map<std::string, double>& variables) const override {
        return sin(children[0]->evaluate(variables));
    }

    Node* getDefaultChild() const override {
        return new NumberNode(0);
    }
};

class CosNode : public OperatorNode {
public:
    CosNode(const std::string operatorSymbol = COS_OPERATOR) : OperatorNode(operatorSymbol, COS_ARGUMENTS) {}

    double evaluate(const std::map<std::string, double>& variables) const override {
        return cos(children[0]->evaluate(variables));
    }

    Node* getDefaultChild() const override {
        return new NumberNode(0);
    }
};