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

ASTParser::ASTParser(const std::string& expresion, std::map<char, int> operatorPriority):
	_expresion(expresion),
	_currentCharacter(_expresion.cbegin()),
	_operatorPriority(operatorPriority)
{
	_expresion.erase(std::remove_if(_expresion.begin(), _expresion.end(), ::isspace), _expresion.end());// removing white spaces
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
	throw std::invalid_argument("Incorrect character");
	return nullptr;
}

std::unique_ptr<IASTNode> ASTParser::ParseParenthesis()
{
	++_currentCharacter;// delete '('
	auto parenthesisExpresion = ParseString();
	if (*_currentCharacter != ')')
	{
		throw std::invalid_argument("Incorrect character");
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
	int currentOperatorPriority = _operatorPriority.at(op);
	auto subTree = ParseSubExpresion();
	subTree = RollUpMulOrDiv(std::move(subTree));
	int nextOperatorPriority = -1;
	if (EndOfExpresion()) // warunek koñcz¹cy rekursywne wywolywanie
	{
		switch (op)
		{
		case '+':
			return std::make_unique< BinaryOperator<std::plus<void>>>(std::move(leftArg), std::move(subTree));
		case '-':
			return std::make_unique< BinaryOperator<std::minus<void>>>(std::move(leftArg), std::move(subTree));
		case '*':
			return std::make_unique< BinaryOperator<std::multiplies<void>>>(std::move(leftArg), std::move(subTree));
		case '/':
			return std::make_unique< BinaryOperator<std::divides<void>>>(std::move(leftArg), std::move(subTree));
		default:
			throw std::invalid_argument("Nieznany operator");
			break;
		}
	}
	switch (op)
	{
	case '+':
		return std::make_unique< BinaryOperator<std::plus<void>>>(std::move(leftArg), ParseBinaryOperator(std::move(subTree)));
	case '-':
		return std::make_unique< BinaryOperator<std::minus<void>>>(ParseBinaryOperator(std::move(leftArg)), std::move(subTree));
	case '*':
		return std::make_unique< BinaryOperator<std::multiplies<void>>>(std::move(leftArg), ParseBinaryOperator(std::move(subTree)));
	case '/':
		return std::make_unique< BinaryOperator<std::divides<void>>>(std::move(leftArg), ParseBinaryOperator(std::move(subTree)));
	default:
		throw std::invalid_argument("Nieznany operator");
		break;
	}

	return std::unique_ptr<IASTNode>();
}

std::unique_ptr<IASTNode> ASTParser::ParseMulOrDivOperator(std::unique_ptr<IASTNode> leftArg)
{
	char op = *_currentCharacter;
	++_currentCharacter;
	int currentOperatorPriority = _operatorPriority.at(op);
	auto subTree = ParseSubExpresion();
	switch (op)
	{
	case '*':
		return std::make_unique< BinaryOperator<std::multiplies<void>>>(std::move(leftArg), std::move(subTree));
	case '/':
		return std::make_unique< BinaryOperator<std::divides<void>>>(std::move(leftArg), std::move(subTree));
	}
	return std::unique_ptr<IASTNode>();
}

std::unique_ptr<IASTNode> ASTParser::RollUpMulOrDiv(std::unique_ptr<IASTNode> leftArg)
{
	while (!EndOfExpresion() &&(*_currentCharacter == '*' || *_currentCharacter == '/'))
	{
		leftArg = ParseMulOrDivOperator(std::move(leftArg));
		if (EndOfExpresion()) // warunek koñcz¹cy rekursywne wywolywanie
		{
			return leftArg;
		}

	}
	return leftArg;
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
