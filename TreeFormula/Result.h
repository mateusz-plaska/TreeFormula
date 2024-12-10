#pragma once

#include <vector>
using namespace std;

template<typename T, typename E>
class Result {
public:
	Result(const T& value) : value(new T(value)), errors() {}
	Result(const E& error) : value(nullptr) {
		this->errors.push_back(new E(error));
	}
	Result(const E* error) : Result(*error) {}
	Result(const vector<E*>& errors) : value(nullptr) {
		this->copyErrors(errors);
	}
	Result(const Result<T, E>& other) {
		if (other.value) {
			this->value = new T(*other.value);
		}
		else {
			this->value = nullptr;
		}

		this->copyErrors(other.errors);
	}
	Result(const T& value, const vector<E*>& errors) : value(new T(value)) {
		this->copyErrors(errors);
	}
	

	~Result() {
		releaseMemory();
	}

	static Result<T, E> ok(const T& value) {
		return Result<T, E>(value);
	}
	static Result<T, E> fail(const E error) {
		return Result<T, E>(error);
	}
	static Result<T, E> fail(const E* error) {
		return Result<T, E>(error);
	}
	static Result<T, E> fail(const vector<E*>& errors) {
		return Result<T, E>(errors);
	}
	static Result<T, E> okWithErrors(const T& value, const vector<E*>& errors) {
		return Result<T, E>(value, errors);
	}

	Result<T, E>& operator=(const Result<T, E>& other) {
		if (this != &other) {
			releaseMemory();
			if (other.value) {
				this->value = new T(*other.value);
			}
			else {
				this->value = nullptr;
			}

			this->copyErrors(other.errors);
		}
		return *this;
	}

	bool isSuccess() const {
		return errors.empty() && value != nullptr;
	}

	T getValue() const {
		if (value) {
			return *value;
		}
		return T();
	}

	const vector<E*>& getErrors() const {
		return errors;
	}

private:
	T* value;
	vector<E*> errors;

	void copyErrors(const vector<E*>& otherErrors) {
		for (E* e : otherErrors) {
			this->errors.push_back(new E(*e));
		}
	}

	void releaseMemory() {
		delete value;
		for (E* e : errors) {
			delete e;
		}
		errors.clear();
	}
};


template<typename E>
class Result<void, E> {
public:
	Result() : success(true), errors() {}
	Result(const E& error) : success(false) {
		this->errors.push_back(new E(error));
	}
	Result(const E* error) : Result(*error) {}
	Result(const vector<E*>& errors) : success(false) {
		this->copyErrors(errors);
	}
	Result(const Result<void, E>& other) : success(other.success) {
		this->copyErrors(other.errors);
	}
	Result(const bool success, const vector<E*>& errors) : success(success) {
		this->copyErrors(errors);
	}

	~Result() {
		releaseMemory();
	}

	static Result<void, E> ok() {
		return Result<void, E>();
	}
	static Result<void, E> fail(const E error) {
		return Result<void, E>(error);
	}
	static Result<void, E> fail(const E* error) {
		return Result<void, E>(error);
	}
	static Result<void, E> fail(const vector<E*>& errors) {
		return Result<void, E>(errors);
	}
	static Result<void, E> okWithErrors(const vector<E*>& errors) {
		return Result<void, E>(true, errors);
	}

	Result<void, E>& operator=(const Result<void, E>& other) {
		if (this != &other) {
			releaseMemory();
			this->success = other.success;
			this->copyErrors(other.errors);
		}
		return *this;
	}

	bool isSuccess() const {
		return success;
	}

	const vector<E*>& getErrors() const {
		return errors;
	}

private:
	bool success;
	vector<E*> errors;

	void copyErrors(const vector<E*>& otherErrors) {
		for (E* e : otherErrors) {
			this->errors.push_back(new E(*e));
		}
	}

	void releaseMemory() {
		for (E* e : errors) {
			delete e;
		}
		errors.clear();
	}
};