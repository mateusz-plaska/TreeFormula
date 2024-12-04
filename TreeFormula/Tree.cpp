#include "Tree.h"

#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <vector>
#include <set>
using namespace std;


const OperatorsCreation Tree::operatorsCreation = Tree::getOperatorsCreation();

const OperatorsCreation& Tree::getOperatorsCreation() {
	static OperatorsCreation operators;

	operators[ADD_OPERATOR] = &OperatorNodeFactory::createAdditionNode;
	operators[SUB_OPERATOR] = &OperatorNodeFactory::createSubtractionNode;
	operators[MUL_OPERATOR] = &OperatorNodeFactory::createMultiplicationNode;
	operators[DIV_OPERATOR] = &OperatorNodeFactory::createDividingNode;
	operators[SIN_OPERATOR] = &OperatorNodeFactory::createSinNode;
	operators[COS_OPERATOR] = &OperatorNodeFactory::createCosNode;

	return operators;
}

string Tree::createTree(const string formula) {
	if (root) {
		releaseTreeMemory();
		root = nullptr;
		variables.clear();
	}
	size_t offset = 0;
	this->root = loadNode(formula, &offset);

	if (offset < formula.length()) {
		infoMessages << Messages::formatMessage(Messages::ERROR_EXTRA_ARGS,
			{ {Messages::toReplaceText::pos, to_string(offset)} }) << endl;
	}

	if (!infoMessages.str().empty()) {
		infoMessages << endl << Messages::formatMessage(Messages::FINAL_FORMULA,
			{ {Messages::toReplaceText::formula, this->toString()} }) << endl;
	}

	string errorMsg = infoMessages.str();
	infoMessages.str(Messages::EMPTY_MSG);
	return errorMsg;
}

double Tree::computeValue(map<string, double> variablesValues) const {
	double result = 0;
	if (root) {
		result = root->evaluate(variablesValues);
	}
	return result;
}

string Tree::toString() const {
	if (!root) {
		return Messages::EMPTY_MSG;
	}
	return root->toString();
}

Node* Tree::loadNode(const string formula, size_t* offset) {
	for (size_t i = *offset; isspace(formula[i]) && i < formula.length(); ++i) {
		(*offset)++;
	}

	string characters = "";
	for (size_t i = *offset; !isspace(formula[i]) && i < formula.length(); ++i) {
		characters += formula[i];
		(*offset)++;
	}

	if (characters.empty()) {
		return nullptr;
	}


	if (operatorsCreation.find(characters) != operatorsCreation.end()) {
		OperatorNode* operatorNode = operatorsCreation.at(characters)();

		for (int i = 0; i < operatorNode->getNumberOfArguments(); ++i) {
			Node* childNode = loadNode(formula, offset);
			if (childNode) {
				operatorNode->addChild(childNode);
			}
			else {
				operatorNode->addChild(operatorNode->getDefaultChild());
				infoMessages << Messages::formatMessage(Messages::ERROR_MISSING_ARGS,
					{ {Messages::toReplaceText::operatorStr, operatorNode->getSymbol()}, 
					{Messages::toReplaceText::position, to_string(*offset)} }) << endl;
			}
		}

		return operatorNode;
	}
	else if (all_of(characters.begin(), characters.end(), ::isdigit)) {
		return new NumberNode(stoull(characters));
	}
	else {
		const size_t initialVariableLength = characters.length();
		vector<pair<char, int>> errorCharsWithPos = StringOperations::removeIncorrectVariableChars(characters);

		for (const pair<char, int> charAndPos : errorCharsWithPos) {
			infoMessages << Messages::formatMessage(Messages::ERROR_INVALID_CHAR,
				{ {Messages::toReplaceText::charStr, Messages::EMPTY_MSG + charAndPos.first},
				  {Messages::toReplaceText::pos, to_string((*offset) - initialVariableLength + charAndPos.second)} }) << endl;
		}


		if (StringOperations::hasLetter(characters)) {
			if (VariableNode* variableNode = findVariable(characters)) {
				return variableNode;
			}
	
			VariableNode* variableNode = new VariableNode(characters);
			variables.insert(variableNode);
			return variableNode;
		}
		else {
			infoMessages << Messages::formatMessage(Messages::ERROR_MISSING_VAR_LETTER,
				{ {Messages::toReplaceText::pos, to_string((*offset) - initialVariableLength) } }) << endl;
			return loadNode(formula, offset);
		}
	}
}

set<string> Tree::getVariableNames() const {
	set<string> variableNames;
	for (VariableNode* variableNode : variables) {
		variableNames.insert(variableNode->getSymbol());
	}
	return variableNames;
}

Tree* Tree::copyTree(const Tree& other) const {
	Tree* res = new Tree();
	res->createTree(other.toString());
	return res;
}

VariableNode* Tree::findVariable(std::string variableName) const {
	for (VariableNode* variableNode : variables) {
		if (variableNode->getSymbol().find(variableName) != std::string::npos) {
			return variableNode;
		}
	}
	return nullptr;
}


Tree Tree::mergeWithTree(const Tree& other) const {
	if (!root) return other;
	if (!other.root) return *this;

	Tree result = *copyTree(*this);
	Node* leaf = findFirstLeaf(result.root);

	if (result.root == leaf) {
		delete leaf;
		result = *copyTree(other);
	}
	else {
		result.deleteProperNode(leaf);
		std::size_t offset = 0;
		replaceChildForParent(result.root, leaf, result.loadNode(other.toString(), &offset));
	}
	return result;
}

int Tree::countVariableOccurrences(Node* node, Node* variableNode) const {
	int count = 0;
	if (auto opNode = dynamic_cast<OperatorNode*>(node)) {
		for (Node* child : opNode->getChildren()) {
			count += countVariableOccurrences(child, variableNode);
		}
	}

	if (node == variableNode) {
		count++;
	}
	return count;
}

void Tree::deleteProperNode(Node* node) {
	if (dynamic_cast<VariableNode*>(node)) {
		int variableOccurences = countVariableOccurrences(this->root, node);
		if (!(--variableOccurences)) {
		//if (countVariableOccurrences(this->root, node) <= 1) {	
			VariableNode* variableNodeToErase = findVariable(node->getSymbol());
			auto it = variables.find(variableNodeToErase);
			delete variableNodeToErase;
			variables.erase(it);
		}
	}
	else {
		delete node;
	}
}

Node* Tree::findFirstLeaf(Node* node) const {
	if (!node) return nullptr;

	OperatorNode* opNode = dynamic_cast<OperatorNode*>(node);
	if (!opNode) {
		return node;
	}

	for (Node* child : opNode->getChildren()) {
		Node* leaf = findFirstLeaf(child);
		if (leaf) return leaf;
	}
	return nullptr;
}

void Tree::replaceChildForParent(Node* currentParent, Node* oldChild, Node* newChild) const {
	if (auto opNode = dynamic_cast<OperatorNode*>(currentParent)) {
		std::vector<Node*>& children = opNode->getChildren();
		for (int i = 0; i < children.size(); ++i) {
			if (children[i] == oldChild) {
				children[i] = newChild;
				return;
			}

			if (dynamic_cast<OperatorNode*>(children[i])) {
				replaceChildForParent(children[i], oldChild, newChild);
			}
		}
	}
	return;
}


void Tree::bfs(vector<pair<Node*, int>>& queue, vector<vector<string>>& result) const {
	if (queue.empty()) {
		return;
	}

	Node* currentNode = (*queue.begin()).first;
	int level = (*queue.begin()).second;
	queue.erase(queue.begin());

	if (level >= result.size()) {
		result.emplace_back();
	}
	result[level].push_back(currentNode->getSymbol());

	if (auto opNode = dynamic_cast<OperatorNode*>(currentNode)) {
		for (Node* child : opNode->getChildren()) {
			queue.push_back(make_pair(child, level + 1));
		}
	}

	bfs(queue, result);
}