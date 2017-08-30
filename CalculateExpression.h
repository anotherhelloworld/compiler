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
    T CalculateBinExpression(ExpressionBinOp*);
    T CalculateUnExpression(ExpressionUnOp*);
};


template <class T> T CalculateExpression<T>::Calculate(Expression* exp) {
    if (exp->expressionType == ExpressionType::VAR) {
        return Calculate(((SymbolVar*)table->GetSymbol(((ExpressionIdent*)exp)->symbol->name, pos))->initExpr);
    }
    if (exp->expressionType == ExpressionType::INT) {
        CheckType<int, T>();
        return atoi(((ExpressionInteger*)exp)->val.val.c_str());
    }
    if (exp->expressionType == ExpressionType::BINOP) {
        return CalculateBinExpression((ExpressionBinOp*)exp);
    }
    if (exp->expressionType == ExpressionType::UNOP) {
        return CalculateUnExpression((ExpressionUnOp*)exp);
    }
}

template <class T> template <class X_1, class X_2> void CalculateExpression<T>::CheckType() {
    if (!std::is_same<X_1, T>::value && !std::is_same<X_2, T>::value) {
        throw TypeCheckerException(typeName, pos);
    }
}

template <class T> T CalculateExpression<T>::CalculateBinExpression(ExpressionBinOp* exp) {
    if (exp->operation.token == ADD) {
        CheckType<int, double>();
        return Calculate(exp->right) + Calculate(exp->left);
    }
    if (exp->operation.token == SUB) {
        CheckType<int, double>();
        return Calculate(exp->right) - Calculate(exp->left);
    }
    if (exp->operation.token == MULT) {
        CheckType<int, double>();
        return Calculate(exp->right) * Calculate(exp->left);
    }
    if (exp->operation.token == DIV) {
        CheckType<int, T>();
    }
    if (exp->operation.token == DIVISION) {
        return Calculate(exp->right) / Calculate(exp->left);
    }
    if (exp->operation.token == MOD) {
        CheckType<int, T>();
        return (int)Calculate(exp->right) % (int)Calculate(exp->left);
    }
    if (exp->operation.token == XOR) {
        CheckType<int, T>();
        return (int)Calculate(exp->right) ^ (int)Calculate(exp->left);
    }
    if (exp->operation.token == AND) {
        CheckType<int, T>();
        return (int)Calculate(exp->right) & (int)Calculate(exp->left);
    }
    if (exp->operation.token == BIT_OR) {
        CheckType<int, T>();
        return (int)Calculate(exp->right) | (int)Calculate(exp->left);
    }
    if (exp->operation.token == SHL) {
        CheckType<int, T>();
        return (int)Calculate(exp->right) << (int)Calculate(exp->left);
    }
    if (exp->operation.token == SHR) {
        CheckType<int, T>();
        return (int)Calculate(exp->right) >> (int)Calculate(exp->left);
    }
}

template <class T> T CalculateExpression<T>::CalculateUnExpression(ExpressionUnOp* exp) {
    if (exp->operation.token == SUB) {
        CheckType<int, double>();
        return - Calculate(exp->arg);
    }
    if (exp->operation.token == ADD) {
        CheckType<int, double>();
        return + Calculate(exp->arg);
    }
    if (exp->operation.token == NOT) {
        CheckType<int, double>();
        return ! Calculate(exp->arg);
    }
}
#endif
