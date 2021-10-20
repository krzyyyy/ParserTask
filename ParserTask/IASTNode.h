#pragma once

class IASTNode
{
public:
	virtual int Evaluate() const = 0;
	virtual ~IASTNode() {};
};