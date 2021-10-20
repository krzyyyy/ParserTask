#include "pch.h"
#include <functional>
#include <memory>
#include "..\ParserTask\BinaryOperator.h"
#include "..\ParserTask\Number.h"

auto make_number = [](int num)
{
	return std::make_unique<Number>(num);
};
using BasePtr = std::unique_ptr<IASTNode>;


TEST(SimpleCases, BinaryOperatorDivides)
{
	using Divides = BinaryOperator<std::divides<void>>;
	auto TestDivides = [](int leftArg, int rightArg)
	{
		return Divides(make_number(leftArg), make_number(rightArg)).Evaluate();
	};
	EXPECT_EQ(TestDivides(1, 3), 0);
	EXPECT_EQ(TestDivides(0, 1), 0);
	EXPECT_EQ(TestDivides(100, -100), -1);
	EXPECT_EQ(TestDivides(-450, 90), -5);
	//divide by zero
	EXPECT_THROW(TestDivides(1, 0), std::invalid_argument); // slabe bo kastuje do std::exception

}
TEST(SimpleCases, BinaryOperatorPlus) 
{
	using Plus = BinaryOperator<std::plus<void>>;
	auto TestPlus = [](int leftArg, int rightArg)
	{
		return Plus(make_number(leftArg), make_number(rightArg)).Evaluate();
	};
	EXPECT_EQ(TestPlus(1, -3), -2);
	EXPECT_EQ(TestPlus(0, 0), 0);
	EXPECT_EQ(TestPlus(100, -1000), -900);
	EXPECT_EQ(TestPlus(10, 90), 100);

}
TEST(SimpleCases, BinaryOperatorMinus)
{
	using Minus = BinaryOperator<std::minus<void>>;
	auto TestMinus = [](int leftArg, int rightArg)
	{
		return Minus(make_number(leftArg), make_number(rightArg)).Evaluate();
	};
	EXPECT_EQ(TestMinus(0, 3), -3);
	EXPECT_EQ(TestMinus(0, 0), 0);
	EXPECT_EQ(TestMinus(100, -1000), 1100);
	EXPECT_EQ(TestMinus(10, 90), -80);

}
TEST(SimpleCases, BinaryOperatorMultiplies
)
{
	using Multiplies = BinaryOperator<std::multiplies<void>>;
	auto TestMultiplies = [](int leftArg, int rightArg)
	{
		return Multiplies(make_number(leftArg), make_number(rightArg)).Evaluate();
	};
	EXPECT_EQ(TestMultiplies(1, 3), 3);
	EXPECT_EQ(TestMultiplies(0, 0), 0);
	EXPECT_EQ(TestMultiplies(100, -100), -10000);
	EXPECT_EQ(TestMultiplies(11, 90), 990);

}