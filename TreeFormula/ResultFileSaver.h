#pragma once

#include "Tree.h"
#include "Error.h"
#include "Result.h"
#include "Messages.h"

#include <string>
#include <fstream>
#include <sstream>

class FileSaver {
public:
	static Result<void, Error> saveStringToFile(const std::string& fileName, const std::string& stringToSave) {
		fileWriter.open(fileName, ios::out | ios::app);

		if (!fileWriter) {
			fileWriter.close();
			return Result<void, Error>::fail(Error(Messages::ERROR_FILE_OPEN));
		}

		fileWriter << stringToSave;

		if (!fileWriter) {
			fileWriter.close();
			return Result<void, Error>::fail(Error(Messages::ERROR_FILE_WRITE));
		}
		fileWriter.close();
		return Result<void, Error>::ok();
	}
protected:
	static std::ofstream fileWriter;
};

std::ofstream FileSaver::fileWriter;


template<typename T>
class ResultFileSaver : public FileSaver {
public:
	static Result<void, Error> saveAll(const Result<T, Error>& result, const std::string& fileName) {
		Result<void, Error> saveResult = saveOnlyErrors(result, fileName);
		if (!saveResult.isSuccess()) {
			return saveResult;
		}
		saveResult = saveOnlyValue(result, fileName);
		if (!saveResult.isSuccess()) {
			return saveResult;
		}
		return Result<void, Error>::ok();
	}
	
	static Result<void, Error> saveOnlyValue(const Result<T, Error>& result, const std::string& fileName) {
		ostringstream valueStr;
		valueStr << endl << result.getValue() << endl << endl;
		return saveStringToFile(fileName, valueStr.str());
	}
	
	static Result<void, Error> saveOnlyErrors(const Result<T, Error>& result, const std::string& fileName) {
		ostringstream errors;
		for (const Error* error : result.getErrors()) {
			errors << endl << error->getErrorDescription();
		}
		return saveStringToFile(fileName, errors.str());
	}
};

template<>
Result<void, Error> ResultFileSaver<Tree>::saveOnlyValue(const Result<Tree, Error>& result, const std::string& fileName) {
	ostringstream valueStr;
	valueStr << endl << result.getValue().toString() << endl;
	return saveStringToFile(fileName, valueStr.str());
}

template<>
Result<void, Error> ResultFileSaver<Tree*>::saveOnlyValue(const Result<Tree*, Error>& result, const std::string& fileName) {
	ostringstream valueStr;
	valueStr << endl << result.getValue()->toString() << endl;
	return saveStringToFile(fileName, valueStr.str());
}

template<>
Result<void, Error> ResultFileSaver<void>::saveOnlyValue(const Result<void, Error>& result, const std::string& fileName) {
	ostringstream valueStr;
	valueStr << endl << result.isSuccess() << endl;
	return saveStringToFile(fileName, valueStr.str());
}