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
#include "Generator.h"

class SymbolTable;
class Block;

enum class DeclarationType {
    D_NULL, CONST, TYPE, VAR, RECORD, FUNC, PROCEDURE, LABEL
};

class Symbol {
public:
    Symbol(std::string name = "", DeclarationType declType = DeclarationType::D_NULL): name(name), declType(declType) {};
    Symbol(Symbol* symbol): name(symbol->name), declType(symbol->declType) {};
    virtual void Print(int) {};
    std::string name;
    DeclarationType declType;
    virtual Symbol* GetType();
    bool Same(std::string);
    virtual void Generate(Generator*) {};
    virtual std::string GenerateName() { return ""; };
    virtual int GetSize() { return 0; };
};

class SymbolType : public Symbol {
public:
    Symbol* type;
    DataType dataType;
    SymbolType(std::string name, Symbol* type): Symbol(name, DeclarationType::TYPE), type(type), dataType(DataType::BADTYPE) {};
    SymbolType(std::string name, DataType dataType): Symbol(name, DeclarationType::TYPE), type(nullptr), dataType(dataType) {};
    Symbol* GetType();
    int GetSize();
    std::string GenerateName();
    void Print(int);
};

class SymbolIdent: public Symbol {
public:
    SymbolIdent(std::string name, DeclarationType declType, Expression* initExpr, Symbol* type, ArgTypeState state):
            Symbol(name, declType), initExpr(initExpr), type(type), state(state) {};
    Symbol* type;
    Expression* initExpr;
    Symbol* GetType();
    void Generate(Generator*);
    std::string GenerateName();
    std::string GetInitlist();
    int GetSize();
    int offset;
    bool localFlag;
    ArgTypeState state;
    int depth = 0;
};

class SymbolConst: public SymbolIdent {
public:
    SymbolConst(std::string name, Expression* initExpr, Symbol* type, ArgTypeState state):
            SymbolIdent(name, DeclarationType::CONST, initExpr, type, state) {};
    void Print(int);
};

class SymbolPointer: public Symbol {
public:
    SymbolPointer(std::string name, Symbol* type): Symbol(name, DeclarationType::TYPE), type(type) {};
//    SymbolPointer(std::string name, Symbol* type): SymbolType(name, type) {};
    Symbol* type;
    void Print(int);
    std::string GenerateName();
    int GetSize();
    Symbol* GetType();
};

class SymbolVar: public SymbolIdent {
public:
    SymbolVar(std::string name, Expression* initExpr, Symbol* type, ArgTypeState state):
            SymbolIdent(name, DeclarationType ::VAR, initExpr, type, state) {};
    void Print(int);
};

class SymbolDynArray : public Symbol {
public:
    Symbol* type;
    DataType dataType;
    SymbolDynArray(Symbol* type): Symbol("", DeclarationType::TYPE), type(type), dataType(DataType::ARRAY) {};
    void Print(int);
    Symbol* GetType();
    virtual int GetLow() { return 0; };
    virtual int GetHigh() { return 0; };
};

class SymbolArray: public SymbolDynArray {
public:
    Symbol* type;
    DataType dataType;
    int left;
    int right;
    SymbolArray(Symbol* type, int left, int right): 
        SymbolDynArray(type), dataType(DataType::ARRAY), type(type), left(left), right(right) {};
    void Print(int);
    int GetLow();
    int GetHigh();
    int GetSize();
    Symbol* GetType();
    std::string GenerateName();
};

class SymbolRecord: public Symbol {
public:
    SymbolTable* table;
    int argc;
    SymbolRecord(SymbolTable* table, std::string name, int argc):
            Symbol(name, DeclarationType::RECORD), table(table), argc(argc) {};
    void Print(int);
    std::string GenerateName();
    int GetSize();
};

class SymbolCall: public Symbol {
public:
    SymbolCall(DeclarationType declType, std::string name, SymbolTable* symbolTable, Block* block, int argc):
            Symbol(name, declType), symbolTable(symbolTable), block(block), argc(argc) {};
    void Generate(Generator*);
    std::string GenerateName();
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
    Symbol* GetType();
    int GetSize();
};

class SymbolProcedure: public SymbolCall {
public:
    SymbolProcedure(std::string name, SymbolTable* symbolTable, Block* block, int argc):
            SymbolCall(DeclarationType::PROCEDURE, name, symbolTable, block, argc) {};
    void Print(int);
};

class SymbolLabel: public Symbol {
public:
    SymbolLabel(std::string name): Symbol(name, DeclarationType::LABEL) {};
    void Print(int);
};

class SymbolString: public SymbolType {
public:
    int length;
    SymbolString(int length): SymbolType("", DataType::STRING), length(length) {};
    void Print(int);
};

class SymbolTable {
public:
    SymbolTable(SymbolTable*);
    void Print(int);
    void Add(Symbol*);
    Symbol* GetSymbol(std::string, std::pair<int, int>);
    void CheckLocalSymbol(std::string, std::pair<int, int>);
    int FindSymbol(std::string);
    Symbol* FindReqSymbol(Expression*, std::pair<int, int>);
    void GenerateVars(Generator*);
    std::pair <int, int> GenerateLocalVariables(Generator*, int, int, int);
    bool Find(std::string);
    std::vector<Symbol*> GetAllSymbols(std::string, std::pair<int, int>);
    std::vector<Symbol*> symbols;
    int stdTypeCount;
private:
    SymbolTable* parentTable;
};

#endif //COMPILER_SYMBOL_H
