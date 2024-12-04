#pragma once
#include "Node.h"
#include "OperatorNode.h"
#include "OperatorsNodes.h"
#include "OperatorNodeFactory.h"
#include "NumberNode.h"
#include "VariableNode.h"
#include "Messages.h"
#include "StringOperations.h"

#include <string>
#include <map>
#include <sstream>
#include <vector>
#include <set>

#include <iostream>

using OperatorsCreation = std::map<const std::string, OperatorNode* (*)()>;

class Tree {
public:
	Tree() : root(nullptr), variables() {}
	Tree(const Tree& other) {
		assignCopiedTree(other);
	}
	~Tree() {
		releaseTreeMemory();
	}

	std::string createTree(const std::string formula);
	std::set<std::string> getVariableNames() const;
	double computeValue(std::map<std::string, double> variablesValues) const;
	std::string toString() const;

	void operator=(const Tree& other) {
		releaseTreeMemory();
		assignCopiedTree(other);
	}
	Tree operator+(const Tree& otherTree) const {
		return this->mergeWithTree(otherTree);
	}

	std::string join(const std::string formula) {
		Tree other;
		std::string errorMsg = other.createTree(formula);
		*this = *this + other;
		return errorMsg;
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
	std::ostringstream infoMessages;
	std::set<VariableNode*> variables;
	static const OperatorsCreation operatorsCreation;


	static const OperatorsCreation& getOperatorsCreation();

	void assignCopiedTree(const Tree& treeToCopy) {
		Tree* copiedTree = copyTree(treeToCopy);
		root = copiedTree->root;
		variables = copiedTree->variables;
		infoMessages.str(copiedTree->infoMessages.str());
	}
	void releaseTreeMemory() {
		delete root;
		for (VariableNode* variable : variables) {
			delete variable;
		}
	}
	Node* loadNode(const std::string formula, std::size_t* offset);
	Tree* copyTree(const Tree& other) const;
	VariableNode* findVariable(std::string variableName) const;

	Tree mergeWithTree(const Tree& other) const;
	int countVariableOccurrences(Node* node, Node* variableNode) const;
	void deleteProperNode(Node* node);
	Node* findFirstLeaf(Node* node) const;
	void replaceChildForParent(Node* currentParent, Node* oldChild, Node* newChild) const;

	void bfs(std::vector<std::pair<Node*, int>>& queue, std::vector<std::vector<std::string>>& result) const;
};

