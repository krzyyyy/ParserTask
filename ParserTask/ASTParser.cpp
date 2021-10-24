#include <functional>
#include <iostream>
#include "ASTParser.h"
#include "BinaryOperator.h"
#include "Number.h"

std::unique_ptr<IASTNode> ASTParser::ParseString()
{
	auto leftArg = ParseSubExpresion();
	leftArg = RollUpMulOrDiv(std::move(leftArg));
    return ParseBinaryOperator(std::move(leftArg));
}

ASTParser::ASTParser(const std::string& expresion):
	_expresion(expresion),
	_currentCharacter(_expresion.cbegin())
{
	_expresion.erase(std::remove_if(_expresion.begin(), _expresion.end(), ::isspace), _expresion.end());// removing white spaces
}

void ASTParser::ThrowExceptionWithMessage() const
{
	auto exceptionMessage = std::string(_expresion.cbegin(), _currentCharacter);
	exceptionMessage += " <------ Invalid syntax";
	throw std::invalid_argument(exceptionMessage);
}

bool ASTParser::EndOfExpresion() const
{
	return (_currentCharacter == _expresion.cend() || *_currentCharacter == ')');
}

std::unique_ptr<IASTNode> ASTParser::ParseSubExpresion()
{
	if (std::isdigit(*_currentCharacter))
	{
		return ParseNumber();
	}
	else if (*_currentCharacter == '(')
	{
		return ParseParenthesis();
	}
	ThrowExceptionWithMessage();
	return nullptr;
}

std::unique_ptr<IASTNode> ASTParser::ParseParenthesis()
{
	++_currentCharacter;// delete '('
	auto parenthesisExpresion = ParseString();
	if (*_currentCharacter != ')')
	{
		ThrowExceptionWithMessage();
	}
	++_currentCharacter;//delete ')'
	return parenthesisExpresion;
}

std::unique_ptr<IASTNode> ASTParser::ParseNumber()
{
	char number = *_currentCharacter;
	//nastêpna pozycja
	++_currentCharacter;
	return std::make_unique<Number>(number - '0');
}

std::unique_ptr<IASTNode> ASTParser::ParseBinaryOperator(std::unique_ptr<IASTNode> leftArg)
{
	if (EndOfExpresion()) // warunek koñcz¹cy rekursywne wywolywanie
	{
		return leftArg;
	}

	char op = *_currentCharacter;
	++_currentCharacter;
	auto subTree = ParseSubExpresion();
	subTree = RollUpMulOrDiv(std::move(subTree));
	switch (op)
	{
	case '+':
		return std::make_unique< BinaryOperator<std::plus<void>>>(std::move(leftArg), ParseBinaryOperator(std::move(subTree)));
	case '-':
		return std::make_unique< BinaryOperator<std::minus<void>>>(ParseBinaryOperator(std::move(leftArg)), std::move(subTree));
	default:
		ThrowExceptionWithMessage();
		break;
	}

	return nullptr;
}

std::unique_ptr<IASTNode> ASTParser::ParseMulOrDivOperator(std::unique_ptr<IASTNode> leftArg)
{
	char op = *_currentCharacter;
	++_currentCharacter;
	auto subTree = ParseSubExpresion();
	switch (op)
	{
	case '*':
		return std::make_unique< BinaryOperator<std::multiplies<void>>>(std::move(leftArg), std::move(subTree));
	case '/':
		return std::make_unique< BinaryOperator<std::divides<void>>>(std::move(leftArg), std::move(subTree));
	default:
	{
		ThrowExceptionWithMessage();
	}
	}
	return std::unique_ptr<IASTNode>();
}

std::unique_ptr<IASTNode> ASTParser::RollUpMulOrDiv(std::unique_ptr<IASTNode> leftArg)
{
	while (!EndOfExpresion() &&(*_currentCharacter == '*' || *_currentCharacter == '/'))
	{
		leftArg = ParseMulOrDivOperator(std::move(leftArg));
	}
	return leftArg;
}

