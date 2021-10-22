#pragma once
#include <memory>
#include "IASTNode.h"

template<typename Op>
class BinaryOperator :
    public IASTNode
{
public:
    inline BinaryOperator(std::unique_ptr<IASTNode> leftArg, std::unique_ptr<IASTNode> rightArg);
    inline int Evaluate() const override;
private:
    std::unique_ptr<IASTNode> _leftArg;
    std::unique_ptr<IASTNode> _rightArg;

    // Inherited via IASTNode

};


template<typename Op>
inline BinaryOperator<typename Op>::BinaryOperator(std::unique_ptr<IASTNode> leftArg, std::unique_ptr<IASTNode> rightArg) :
    _leftArg(std::move(leftArg)),
    _rightArg(std::move(rightArg))
{
}

template<typename Op>
inline int BinaryOperator<Op>::Evaluate() const
{
    return Op()(_leftArg->Evaluate(), _rightArg->Evaluate());
}

template<>
inline int BinaryOperator<std::divides<void>>::Evaluate()const
{
    int leftValue = _leftArg->Evaluate();
    int rightValue = _rightArg->Evaluate();
    if (rightValue == 0)
    {
        throw std::invalid_argument("Gdzies dzielimy przez 0 :( ");
    }
    return leftValue / rightValue;
}


