#ifndef COMPILER_TYPECHECKER_H
#define COMPILER_TYPECHECKER_H

#include "Expression.h"
//#include "Symbol.h"

class SymbolTable;
class Symbol;

enum class DataType {
    BADTYPE = -1, INTEGER = 0, REAL = 1, CHAR = 2, BOOLEAN = 3, ARRAY = 4, STRING = 5, RECORD = 6, POINTER = 7, 
};

static std::vector<std::string> dataTypeString = {
        "integer", "real", "char", "boolean", "array", "string", "record", "pointer"
};

class TypeChecker {
public:
    TypeChecker(SymbolTable*, Expression*, std::pair<int, int>);
    TypeChecker(SymbolTable* symbolTable, std::pair<int, int> pos): symbolTable(symbolTable), pos(pos) {};
    TypeChecker(SymbolTable*, DataType, Expression*, std::pair<int, int>);
    TypeChecker(SymbolTable*, Symbol*, Expression*, std::pair<int, int>);
    TypeChecker(SymbolTable*, Expression*, Expression*, std::pair <int, int>);
    void Check(DataType, DataType);
    bool CanCast(DataType, DataType);
    DataType GetTypeID(Expression*);
private:
    DataType GetTypeID(ExpressionType);
    DataType GetTypeIDBinExpression(DataType, DataType, TokenType);
    SymbolTable* symbolTable;
    std::pair<int, int> pos;
};

class CmpArguments {
public:
    bool Compare(Symbol*, Symbol*);
private:
    bool CompareTypes(Symbol*, Symbol*);
};

#endif //COMPILER_TYPECHECKER_H
