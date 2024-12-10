#pragma once

#include "Result.h"
#include "Error.h"
#include "Messages.h"

#include <map>
#include<string>
#include <sstream>
#include <vector>


class Node {
public:
	virtual ~Node() {};

	virtual Result<double, Error> evaluate(const std::map<std::string, double>& variables) const = 0;
	virtual std::string getSymbol() const = 0;

	virtual std::string toString() const {
		std::ostringstream result;
		result << getSymbol() << Messages::SPACE_MSG;
		return result.str();
	}

	virtual bool isOperatorNode() const = 0;
};

