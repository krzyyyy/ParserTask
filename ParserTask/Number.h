#pragma once
#include "IASTNode.h"
class Number :
    public IASTNode
{
public:
    inline Number(int number) :
        _number(number){};
    // Inherited via IASTNode
    inline int Evaluate() const override
    {
        return _number;
    };

private:
    int _number;
};

