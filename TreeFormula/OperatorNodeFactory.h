#pragma once
#include "OperatorsNodes.h"
#include "OperatorNode.h"

#include <string>

class OperatorNodeFactory {
public:
	static OperatorNode* createAdditionNode() {
		return new AdditionNode();
	}
	static OperatorNode* createSubtractionNode() {
			return new SubtractionNode();
		}
	static OperatorNode* createMultiplicationNode() {
			return new MultiplicationNode();
		}
	static OperatorNode* createDividingNode() {
			return new DividingNode();
		}
	static OperatorNode* createSinNode() {
			return new SinNode();
		}
	static OperatorNode* createCosNode() {
			return new CosNode();
		}
};

