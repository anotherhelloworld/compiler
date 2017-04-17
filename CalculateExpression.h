//
// Created by Александр on 17.04.17.
//

#ifndef COMPILER_CALCULATEEXPRESSION_H
#define COMPILER_CALCULATEEXPRESSION_H

#include "Expression.h"
#include "Symbol.h"

class CalculateExpression {
public:
    int Ans;
    SymbolTable* table;
    std::string typeName;
    std::pair<int, int> pos;
    CalculateExpression(SymbolTable* table, std::string typeName, std::pair<int, int> pos): table(table), typeName(typeName), pos(pos) {};
    int Calculate(Expression* exp);
};

int CalculateExpression::Calculate(Expression* exp) {
}

#endif //COMPILER_CALCULATEEXPRESSION_H
