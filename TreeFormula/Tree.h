#pragma once
#include "Node.h"
#include "OperatorNode.h"
#include "OperatorsNodes.h"
#include "OperatorNodeFactory.h"
#include "NumberNode.h"
#include "VariableNode.h"
#include "Messages.h"
#include "StringOperations.h"
#include "Error.h"
#include "Result.h"

#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <set>

const std::string FILE_NAME = "Tree.txt";
const std::string FILE_NAME_ERROR = "Tree_WriteError.txt";

using OperatorsCreation = std::map<const std::string, OperatorNode* (*)()>;

class Tree {
public:
	Tree() : root(nullptr), variables() {}
	Tree(const Tree& other) {
		assignCopiedTree(other);
	}
	Tree(Tree&& other) noexcept {
		root = other.root;
		variables = other.variables;
		errors = other.errors;

		other.root = nullptr;
		other.variables.clear();
		other.errors.clear();
	}
	~Tree() {
		releaseTreeMemory();
	}

	Result<Tree*, Error> createTree(const std::string formula);
	std::set<std::string> getVariableNames() const;

	Result<double, Error> computeValue(const std::map<std::string, double>& variablesValues) const;
	std::string toString() const;
	Result<Tree*, Error> join(const std::string formula);

	Tree operator=(Tree&& other) noexcept {
		if (this != &other) {
			releaseTreeMemory();

			root = other.root;
			variables = other.variables;
			errors = other.errors;

			other.root = nullptr;
			other.variables.clear();
			other.errors.clear();
		}
		return *this;
	}

	Tree operator=(const Tree& other) {
		if (this != &other) {
			releaseTreeMemory();
			assignCopiedTree(other);
		}
		return *this;
	}

	Tree operator+(const Tree& otherTree) const {
		return this->mergeWithTree(otherTree);
	}

	std::vector<std::vector<std::string>> getTreeRows() const {
		std::vector<std::vector<std::string>> result;
		std::vector<std::pair<Node*, int>> queue;
		queue.push_back(std::make_pair(root, 0));
		bfs(queue, result);
		return result;
	}

private:
	Node* root;
	std::set<VariableNode*> variables;
	std::vector<Error*> errors;
	static const OperatorsCreation operatorsCreation;

	static const OperatorsCreation& getOperatorsCreation();

	void assignCopiedTree(const Tree& treeToCopy) {
		Tree* copiedTree = copyTree(treeToCopy);
		root = copiedTree->root;
		variables = copiedTree->variables;
		errors = copiedTree->errors;
	}
	void releaseTreeMemory() {
		if (!dynamic_cast<VariableNode*>(root)) {
			delete root;
		}

		for (VariableNode* variable : variables) {
			delete variable;
		}
		variables.clear();
		
		root = nullptr;

		releaseErrors();
	}
	void releaseErrors() {
		for (Error* error : errors) {
			delete error;
		}
		errors.clear();
	}

	Node* loadNode(const std::string formula, std::size_t* offset);
	Tree* copyTree(const Tree& other) const;

	VariableNode* findVariable(const std::string& variableName) const;

	Tree mergeWithTree(const Tree& other) const;
	int countVariableOccurrences(Node* node, Node* variableNode) const;
	void deleteProperNode(Node* node);
	Node* findFirstLeaf(Node* node) const;
	void replaceChildForParent(Node* currentParent, Node* oldChild, Node* newChild) const;

	void bfs(std::vector<std::pair<Node*, int>>& queue, std::vector<std::vector<std::string>>& result) const;

	
	void createTreeLog(const Result<Tree*, Error>& result, const string& formula) const;
	void computeLog(const Result<double, Error>& result, const map<string, double>& variablesValues) const;
	void joinLog() const;
};

