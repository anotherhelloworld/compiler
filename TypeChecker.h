#ifndef COMPILER_TYPECHECKER_H
#define COMPILER_TYPECHECKER_H

#include "Expression.h"
//#include "Symbol.h"

class SymbolTable;
class Symbol;

//enum class TypeID {
//    BAD, INTEGER, REAL, CHAR, BOOLEAN,
//};

enum class DataType {
    BADTYPE = -1, INTEGER = 0, REAL = 1, CHAR = 2, ARRAY = 3,
};

class TypeChecker {
public:
    DataType GetTypeID(Expression*);
    TypeChecker(SymbolTable* symbolTable, Expression* expr, std::pair<int, int> pos);
    TypeChecker(SymbolTable* symbolTable, std::pair<int, int> pos): symbolTable(symbolTable), pos(pos) {};
    void Check(DataType, DataType);
    bool CanCast(DataType, DataType);
private:
    DataType GetTypeID(ExpressionType);
    SymbolTable* symbolTable;
    std::pair<int, int> pos;
};

#endif //COMPILER_TYPECHECKER_H
