#pragma once

#include <vector>
#include <string>

namespace ASCIICodes {
	const char UPPERCASE_A = 'A';
	const char UPPERCASE_Z = 'Z';
	const char LOWERCASE_A = 'a';
	const char LOWERCASE_Z = 'z';
};

namespace StringOperations {
	static bool isLowerCaseLetter(const char c) {
		return (c >= ASCIICodes::LOWERCASE_A && c <= ASCIICodes::LOWERCASE_Z);
	}

	static bool isUpperCaseLetter(const char c) {
		return (c >= ASCIICodes::UPPERCASE_A && c <= ASCIICodes::UPPERCASE_Z);
	}

	static std::vector<std::pair<char, int>> removeIncorrectVariableChars(std::string& variableName) {
		std::vector<std::pair<char, int>> errorCharsWithPosition;
		std::string result;
		for (int i = 0; i < variableName.length(); ++i) {
			if (isUpperCaseLetter(variableName[i]) || isLowerCaseLetter(variableName[i]) || isdigit(variableName[i])) {
				result += variableName[i];
			}
			else {
				errorCharsWithPosition.push_back(std::make_pair(variableName[i], i));
			}
		}
		variableName = result;
		return errorCharsWithPosition;
	}

	static bool hasLetter(const std::string& variableName) {
		for (char c : variableName) {
			if (isUpperCaseLetter(c) || isLowerCaseLetter(c)) {
				return true;
			}
		}
		return false;
	}
};
