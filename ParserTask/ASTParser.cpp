#include <functional>
#include <iostream>
#include "ASTParser.h"
#include "BinaryOperator.h"
#include "Number.h"

std::unique_ptr<IASTNode> ASTParser::ParseString()
{

    return ParseBinaryOperator(ParseNumber());
}

ASTParser::ASTParser(const std::string& expresion):
	_expresion(expresion),
	_currentCharacter(_expresion.cbegin())
{

}

std::unique_ptr<IASTNode> ASTParser::ParseNumber()
{
	char number = *_currentCharacter;
	if (!std::isdigit(number))
		throw std::invalid_argument("  fsdf");
	//nastêpna pozycja
	++_currentCharacter;
	return std::make_unique<Number>(number - '0');
}

std::unique_ptr<IASTNode> ASTParser::ParseBinaryOperator(std::unique_ptr<IASTNode> leftArg)
{
	if (_currentCharacter == _expresion.cend()) // warunek koñcz¹cy rekursywne wywolywanie
	{
		return leftArg;
	}
	char op = *_currentCharacter;
	++_currentCharacter;
	switch (op)
	{
	case '+':
		return std::make_unique< BinaryOperator<std::plus<void>>>(std::move(leftArg), ParseBinaryOperator(ParseNumber()));
	case '-':
		return std::make_unique< BinaryOperator<std::minus<void>>>( ParseBinaryOperator(std::move(leftArg)), ParseNumber());
	case '*':
		return std::make_unique< BinaryOperator<std::multiplies<void>>>(std::move(leftArg), ParseBinaryOperator(ParseNumber()));
	case '/':
		return std::make_unique< BinaryOperator<std::divides<void>>>(std::move(leftArg), ParseBinaryOperator(ParseNumber()));
	default:
		throw std::invalid_argument("Nieznany operator");
		break;
	}
	return std::unique_ptr<IASTNode>();
}

std::unique_ptr<IASTNode> ASTParser::MakeBinaryOp(std::unique_ptr<IASTNode> leftArg, std::unique_ptr<IASTNode> rightArg, char op) const
{
	switch (op)
	{
	case '+':
		return std::make_unique< BinaryOperator<std::plus<void>>>(std::move(leftArg), std::move(rightArg));
	case '-':
		return std::make_unique< BinaryOperator<std::minus<void>>>(std::move(leftArg), std::move(rightArg));
	case '*':
		return std::make_unique< BinaryOperator<std::multiplies<void>>>(std::move(leftArg), std::move(rightArg));
	case '/':
		return std::make_unique< BinaryOperator<std::divides<void>>>(std::move(leftArg), std::move(rightArg));
	default:
		throw std::invalid_argument("Nieznany operator");
		break;
	}
    return nullptr;
}
