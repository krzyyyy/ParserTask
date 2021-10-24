#pragma once
#include <memory>
#include <string>
#include <map>
#include "IASTNode.h"


class ASTParser
{
public:
	ASTParser(const std::string& expresion);
	std::unique_ptr<IASTNode> ParseString();

private:
	std::string _expresion;
	std::string::const_iterator _currentCharacter;
	bool EndOfExpresion() const;
	std::unique_ptr<IASTNode> ParseSubExpresion(); // parse number or parenthesis 
	std::unique_ptr<IASTNode> ParseParenthesis(); // parse number or parenthesis 
	std::unique_ptr<IASTNode> ParseNumber();
	std::unique_ptr<IASTNode> ParseBinaryOperator(std::unique_ptr<IASTNode> leftArg);
	std::unique_ptr<IASTNode> ParseMulOrDivOperator(std::unique_ptr<IASTNode> leftArg);
	std::unique_ptr<IASTNode> RollUpMulOrDiv(std::unique_ptr<IASTNode> leftArg);
};

