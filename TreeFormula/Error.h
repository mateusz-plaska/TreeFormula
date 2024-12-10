#pragma once

#include <string>

class Error {
public:
	Error(const std::string& errorMessage) : errorMessage(errorMessage) {}

	const std::string getErrorDescription() const {
		return errorMessage;
	}

private:
	std::string errorMessage;
};