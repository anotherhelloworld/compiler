#ifndef COMPILER_SYMBOL_H
#define COMPILER_SYMBOL_H
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
//#include "Block.h"
#include "Errors.h"
#include "TypeChecker.h"

class SymbolTable;
class Block;

enum class DeclarationType {
    D_NULL, CONST, TYPE, VAR, RECORD, FUNC, PROCEDURE,
};

class Symbol {
public:
    Symbol(std::string name, DeclarationType declType): name(name), declType(declType) {};
    Symbol(Symbol* symbol): name(symbol->name), declType(symbol->declType) {};
    virtual void Print(int) {};
    std::string name;
    DeclarationType declType;
    virtual Symbol* GetType();
    bool Same(std::string);
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
    SymbolIdent(std::string name, DeclarationType declType, Expression* initExpr, Symbol* type):
            Symbol(name, declType), initExpr(initExpr), type(type) {};
    Symbol* type;
    Expression* initExpr;
    Symbol* GetType();
    int offset;
};

class SymbolConst: public SymbolIdent {
public:
    SymbolConst(std::string name, Expression* initExpr, Symbol* type):
            SymbolIdent(name, DeclarationType::CONST, initExpr, type) {};
    void Print(int);
};

class SymbolPointer: public Symbol {
public:
    SymbolPointer(std::string name, Symbol* type): Symbol(name, DeclarationType::TYPE), type(type) {};
    Symbol* type;
    void Print(int);
};

class SymbolVar: public SymbolIdent {
public:
    SymbolVar(std::string name, Expression* initExpr, Symbol* type):
            SymbolIdent(name, DeclarationType ::VAR,initExpr, type) {};
    void Print(int);
};

class SymbolArray: public Symbol {
public:
    Symbol* type;
    DataType dataType;
    Expression* left;
    Expression* right;
    SymbolArray(Symbol* type, Expression* left, Expression* right): Symbol("", DeclarationType::TYPE), dataType(DataType::ARRAY), type(type), left(left), right(right) {};
    void Print(int);
};

class SymbolRecord: public Symbol {
public:
    SymbolTable* table;
    int argc;
    SymbolRecord(SymbolTable* table, std::string name, int argc):
            Symbol(name, DeclarationType::RECORD), table(table), argc(argc) {};
    void Print(int);
};

class SymbolCall: public Symbol {
public:
    SymbolCall(DeclarationType declType, std::string name, SymbolTable* symbolTable, Block* block, int argc):
            Symbol(name, declType), symbolTable(symbolTable), block(block), argc(argc) {};
    SymbolTable* symbolTable;
    Block* block;
    int argc;
};

class SymbolFunction: public SymbolCall {
public:
    SymbolFunction(std::string name, SymbolTable* symbolTable, Block* block, int argc, Symbol* type):
            SymbolCall(DeclarationType::FUNC, name, symbolTable, block, argc), type(type) {};
    Symbol* type;
    void Print(int);
};

class SymbolProcedure: public SymbolCall {
public:
    SymbolProcedure(std::string name, SymbolTable* symbolTable, Block* block, int argc):
            SymbolCall(DeclarationType::PROCEDURE, name, symbolTable, block, argc) {};
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
    std::vector<Symbol*> GetAllSymbols(std::string, std::pair<int, int>);
    std::vector<Symbol*> symbols;
private:
    SymbolTable* parentTable;
    int stdTypeCount;
};

#endif //COMPILER_SYMBOL_H
