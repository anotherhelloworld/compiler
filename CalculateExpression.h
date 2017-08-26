#ifndef COMPILER_CALCULATEEXPRESSION_H
#define COMPILER_CALCULATEEXPRESSION_H

#include "Expression.h"
#include "Symbol.h"
#include <type_traits>

template <class T>
class CalculateExpression {
public:
    T Ans;
    SymbolTable* table;
    std::string typeName;
    std::pair<int, int> pos;
    CalculateExpression(SymbolTable* table, std::string typeName, std::pair<int, int> pos): table(table), typeName(typeName), pos(pos) {};
    T Calculate(Expression* exp);
    template <class X_1, class X_2> void CheckType();
};


template <class T> T CalculateExpression<T>::Calculate(Expression* exp) {
    if (exp->expressionType == ExpressionType::VAR) {
        return Calculate(((SymbolVar*)table->GetSymbol(((ExpressionIdent*)exp)->symbol->name, pos))->initExpr);
    }
    if (exp->expressionType == ExpressionType::INT) {
        CheckType<int, int>();
        return atoi(((ExpressionInteger*)exp)->val.val.c_str());
    }
}

template <class T> template <class X_1, class X_2> void CalculateExpression<T>::CheckType() {
    if (!std::is_same<X_1, T>::value && !std::is_same<X_2, T>::value) {
        throw TypeCheckerException(typeName, pos);
    }
}
#endif //COMPILER_CALCULATEEXPRESSION_H
