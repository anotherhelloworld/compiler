#ifndef COMPILER_SYMBOL_H
#define COMPILER_SYMBOL_H
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Expression.h"

enum class DeclarationType {
    D_NULL, CONST, TYPE, VAR,
};

enum class ArgumentType {
    RVALUE, VARIABLE, CONSTANT, OUT,
};

enum class DataType {
    BADTYPE, INTEGER, DOUBLE, CHAR,
};

class Symbol {
public:
    Symbol(std::string name, DeclarationType declType): name(name), declType(declType) {};
    Symbol(Symbol* symbol): name(symbol->name), declType(symbol->declType) {};
    virtual void Print(int) {};
    std::string name;
    DeclarationType declType;
    virtual Symbol* GetType();
};

class SymbolType : public Symbol {
public:
    Symbol* type;
    DataType dataType;
    SymbolType(std::string name, Symbol* type): Symbol(name, DeclarationType::TYPE), type(type), dataType(DataType::BADTYPE) {};
    SymbolType(std::string name, DataType dataType): Symbol(name, DeclarationType::TYPE), type(nullptr), dataType(dataType) {};
    Symbol* GetType();
    void Print(int);
};

class SymbolIdent: public Symbol {
public:
    ArgumentType argType;
    SymbolIdent(std::string name, DeclarationType declType, Expression* initExpr, Symbol* type, ArgumentType argType):
            Symbol(name, declType), initExpr(initExpr), type(type), argType(argType) {};
    Symbol* type;
    Expression* initExpr;
    Symbol* GetType();
};

class SymbolConst: public SymbolIdent {
public:
    SymbolConst(std::string name, Expression* initExpr, Symbol* type, ArgumentType argType):
            SymbolIdent(name, DeclarationType::CONST, initExpr, type, argType) {};
    void Print(int);
};

class SymPointer: public Symbol {
public:
    SymPointer(std::string name, Symbol* type): Symbol(name, DeclarationType::TYPE), type(type) {};
    Symbol* type;
};

class SymbolVar: public SymbolIdent {
public:
    SymbolVar(std::string name, Expression* initExpr, Symbol* type, ArgumentType argType):
            SymbolIdent(name, DeclarationType ::VAR,initExpr, type, argType) {};
    void Print(int);
};

class SymbolArray: public Symbol {
public:
    Symbol* type;
    DataType dataType;
    Expression* left;
    Expression* right;
    SymbolArray(Symbol* type, Expression* left, Expression* right): Symbol("", DeclarationType::TYPE), type(type), left(left), right(right) {};
    void Print(int);
};

class SymbolTable {
public:
    SymbolTable(SymbolTable*);
    void Print(int);
    void Add(Symbol*);
    Symbol* GetSymbol(std::string, std::pair<int, int>);
    int FindSymbol(std::string);
    void CheckLocalSymbol(std::string, std::pair<int, int>);
    bool Find(std::string);
private:
    SymbolTable* parentTable;
    std::vector<Symbol*> symbols;
    int stdTypeCount;
};

#endif //COMPILER_SYMBOL_H
