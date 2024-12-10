#pragma once
#include "OperatorNode.h"
#include "NumberNode.h"
#include "Result.h"
#include "Error.h"

#include <map>
#include <string>
#include <cmath>

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

	Result<double, Error> evaluate(const std::map<std::string, double>& variables) const override {
        Result<double, Error> result = Result<double, Error>::ok(0);

        for (const auto& child : children) {
            Result<double, Error> childResult = child->evaluate(variables);
            if (!childResult.isSuccess()) {
                return childResult;
            }

            double newValue = result.getValue() + childResult.getValue();
            if (std::isinf(newValue)) {
                return Result<double, Error>::fail(Error(Messages::formatMessage(Messages::ERROR_OVERFLOW,
                    { {Messages::toReplaceText::operatorStr, operatorSymbol} })));
            }
            result = Result<double, Error>::ok(newValue);
        }
        return result;
	}
};

class SubtractionNode : public OperatorNode {
public:
    SubtractionNode(const std::string operatorSymbol = SUB_OPERATOR) : OperatorNode(operatorSymbol, SUB_ARGUMENTS) {}

    Result<double, Error> evaluate(const std::map<std::string, double>& variables) const override {
        Result<double, Error> result = Result<double, Error>::ok(0);

        if (children.size()) {
            result = children.front()->evaluate(variables);
            if (!result.isSuccess()) {
                return result;
            }
        }

        for (int i = 1; i < children.size(); ++i) {
            Result<double, Error> childResult = children[i]->evaluate(variables);
            if (!childResult.isSuccess()) {
                return childResult;
            }

            double newValue = result.getValue() - childResult.getValue();
            if (std::isinf(newValue)) {
                return Result<double, Error>::fail(Error(Messages::formatMessage(Messages::ERROR_OVERFLOW,
                    { {Messages::toReplaceText::operatorStr, operatorSymbol} })));
            }
            result = Result<double, Error>::ok(newValue);
        }
        return result;
    }
};

class MultiplicationNode : public OperatorNode {
public:
    MultiplicationNode(const std::string operatorSymbol = MUL_OPERATOR) : OperatorNode(operatorSymbol, MUL_ARGUMENTS) {}

    Result<double, Error> evaluate(const std::map<std::string, double>& variables) const override {
        Result<double, Error> result = Result<double, Error>::ok(1);
        for (const auto& child : children) {
            Result<double, Error> childResult = child->evaluate(variables);
            if (!childResult.isSuccess()) {
                return childResult;
            }

            double newValue = result.getValue() * childResult.getValue();
            if (std::isinf(newValue)) {
                return Result<double, Error>::fail(Error(Messages::formatMessage(Messages::ERROR_OVERFLOW,
                    { {Messages::toReplaceText::operatorStr, operatorSymbol} })));
            }
            result = Result<double, Error>::ok(newValue);
        }
        return result;
    }
};

class DividingNode : public OperatorNode {
public:
    DividingNode(const std::string operatorSymbol = DIV_OPERATOR) : OperatorNode(operatorSymbol, DIV_ARGUMENTS) {}

    Result<double, Error> evaluate(const std::map<std::string, double>& variables) const override {
        Result<double, Error> result = Result<double, Error>::ok(0);
        if (children.size()) {
            result = children.front()->evaluate(variables);
            if (!result.isSuccess()) {
                return result;
            }
        }

        for (int i = 1; i < children.size(); ++i) {
            Result<double, Error> childResult = children[i]->evaluate(variables);
            if (!childResult.isSuccess()) {
                return childResult;
            }

            if (childResult.getValue() == 0) {
                return Result<double, Error>::fail(Error(Messages::ERROR_DIVISION_BY_0));
            }

            if (childResult.getValue() < 0) {
                return Result<double, Error>::fail(Error(Messages::ERROR_DIVISION_BY_NEGATIVE_NUMBER));
            }

            double newValue = result.getValue() / childResult.getValue();
            if (std::isinf(newValue)) {
                return Result<double, Error>::fail(Error(Messages::formatMessage(Messages::ERROR_OVERFLOW,
                    { {Messages::toReplaceText::operatorStr, operatorSymbol} })));
            }
            result = Result<double, Error>::ok(newValue);
        }
        return result;
    }
};

class SinNode : public OperatorNode {
public:
    SinNode(const std::string operatorSymbol = SIN_OPERATOR) : OperatorNode(operatorSymbol, SIN_ARGUMENTS) {}

    Result<double, Error> evaluate(const std::map<std::string, double>& variables) const override {
        Result<double, Error> childResult = children[0]->evaluate(variables);
        if (!childResult.isSuccess()) {
            return childResult;
        }
        return Result<double, Error>::ok(sin(childResult.getValue()));
    }

    Node* getDefaultChild() const override {
        return new NumberNode(0);
    }
};

class CosNode : public OperatorNode {
public:
    CosNode(const std::string operatorSymbol = COS_OPERATOR) : OperatorNode(operatorSymbol, COS_ARGUMENTS) {}

    Result<double, Error> evaluate(const std::map<std::string, double>& variables) const override {
        Result<double, Error> childResult = children[0]->evaluate(variables);
        if (!childResult.isSuccess()) {
            return childResult;
        }
        return Result<double, Error>::ok(cos(childResult.getValue()));
    }

    Node* getDefaultChild() const override {
        return new NumberNode(0);
    }
};