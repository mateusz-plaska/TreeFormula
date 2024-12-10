#include "Tree.h"
#include "ResultFileSaver.h"

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

Result<Tree*, Error> Tree::createTree(const string formula) {
	if (root) {
		releaseTreeMemory();
	}
	size_t offset = 0;
	this->root = loadNode(formula, &offset);

	if (offset < formula.length() && !(all_of(formula.begin() + offset, formula.end(), ::isspace))) {		
		errors.push_back(new Error(Messages::formatMessage(Messages::ERROR_EXTRA_ARGS,
			{ {Messages::toReplaceText::pos, to_string(offset)} })));
	}

	Result<Tree*, Error> result = Result<Tree*, Error>::ok(this);
	if (!errors.empty()) {
		result = Result<Tree*, Error>::okWithErrors(this, errors);
		releaseErrors();
	}
	createTreeLog(result, formula);
	return result;
}

Result<double, Error> Tree::computeValue(const map<string, double>& variablesValues) const {
	Result<double, Error> result = Result<double, Error>::ok(0);
	if (root) {
		result = root->evaluate(variablesValues);
	}
	computeLog(result, variablesValues);
	return result;
}

string Tree::toString() const {
	if (!root) {
		return Messages::EMPTY_MSG;
	}
	return root->toString();
}

Result<Tree*, Error> Tree::join(const std::string formula) {
	joinLog();
	Tree other;
	Result<Tree*, Error> otherTreeResult = other.createTree(formula);
	*this = *this + other;
	return Result<Tree*, Error>::okWithErrors(this, otherTreeResult.getErrors());
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
				errors.push_back(new Error(Messages::formatMessage(Messages::ERROR_MISSING_ARGS,
					{ {Messages::toReplaceText::operatorStr, operatorNode->getSymbol()},
					{Messages::toReplaceText::position, to_string(*offset)} })));
			}
		}

		return operatorNode;
	}
	else if (all_of(characters.begin(), characters.end(), ::isdigit)) {
		unsigned long long value = 1;
		try {
			value = stoull(characters);
		}
		catch (const exception& e) {
			errors.push_back(new Error(Messages::formatMessage(Messages::ERROR_INVALID_VALUE,
				{ {Messages::toReplaceText::valueStr, characters} })));
		}

		return new NumberNode(value);
	}
	else {
		const size_t initialVariableLength = characters.length();
		vector<pair<char, int>> errorCharsWithPos = StringOperations::removeIncorrectVariableChars(characters);

		for (const pair<char, int> charAndPos : errorCharsWithPos) {
			errors.push_back(new Error(Messages::formatMessage(Messages::ERROR_INVALID_CHAR,
				{ {Messages::toReplaceText::charStr, Messages::EMPTY_MSG + charAndPos.first},
				  {Messages::toReplaceText::pos, to_string((*offset) - initialVariableLength + charAndPos.second)} })));
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
			errors.push_back(new Error(Messages::formatMessage(Messages::ERROR_MISSING_VAR_LETTER,
				{ {Messages::toReplaceText::pos, to_string((*offset) - initialVariableLength) } })));
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

VariableNode* Tree::findVariable(const std::string& variableName) const {
	for (VariableNode* variableNode : variables) {
		if (variableNode->getSymbol() == variableName) {
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
		if (countVariableOccurrences(this->root, node) <= 1) {	
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


void Tree::createTreeLog(const Result<Tree*, Error>& result, const string& formula) const {
	Result<void, Error> saveResult = FileSaver::saveStringToFile(FILE_NAME, Messages::Logs::CREATE_TREE + formula);
	if (!saveResult.isSuccess()) {
		ResultFileSaver<void>::saveAll(saveResult, FILE_NAME_ERROR);
	}
	
	saveResult = ResultFileSaver<Tree*>::saveAll(result, FILE_NAME);
	if (!saveResult.isSuccess()) {
		ResultFileSaver<void>::saveAll(saveResult, FILE_NAME_ERROR);
	}
}

void Tree::computeLog(const Result<double, Error>& result, const map<string, double>& variablesValues) const {	
	string vars = Messages::EMPTY_MSG;
	for (pair<string, double> variableWithValue : variablesValues) {
		vars += variableWithValue.first + Messages::EQUAL_SIGN + to_string(variableWithValue.second) + Messages::SPACE_MSG;
	}

	Result<void, Error> saveResult = FileSaver::saveStringToFile(FILE_NAME, Messages::Logs::COMPUTE + this->toString() + Messages::Logs::VARS + vars);
	if (!saveResult.isSuccess()) {
		ResultFileSaver<void>::saveAll(saveResult, FILE_NAME_ERROR);
	}

	saveResult = ResultFileSaver<double>::saveAll(result, FILE_NAME);
	if (!saveResult.isSuccess()) {
		ResultFileSaver<void>::saveAll(saveResult, FILE_NAME_ERROR);
	}
}

void Tree::joinLog() const {
	Result<void, Error> saveResult = FileSaver::saveStringToFile(FILE_NAME, Messages::Logs::JOIN + this->toString() + "\n");
	if (!saveResult.isSuccess()) {
		ResultFileSaver<void>::saveAll(saveResult, FILE_NAME_ERROR);
	}
}