#pragma once
#include <memory>
#include <string>
#include "IASTNode.h"


class ASTParser
{
public:
	ASTParser(const std::string& expresion);
	std::unique_ptr<IASTNode> ParseString();

private:
	std::string _expresion;
	std::string::const_iterator _currentCharacter;


	std::unique_ptr<IASTNode> ParseNumber();
	std::unique_ptr<IASTNode> ParseBinaryOperator(std::unique_ptr<IASTNode> leftArg);
	std::unique_ptr<IASTNode> MakeBinaryOp(std::unique_ptr<IASTNode> leftArg, std::unique_ptr<IASTNode> rightArg, char op)const;
};

