// ParserTaskExe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "..\ParserTask\ASTParser.h"

int main(int argc, char* argv[])
{
    std::string expresion = "";
    for (int i = 1; i < argc; ++i)
    {
        expresion += std::string(argv[i]);
    }
    auto parser = ASTParser(expresion);
    try
    {
        auto AST = parser.ParseString();
        int result = AST->Evaluate();
        std::cout << result << std::endl;
    }
    catch (const std::invalid_argument& exception)
    {
        std::cout << "Invalid expresion:" << std::endl;
        std::cout << exception.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unnown error" << std::endl;
    }
    
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
