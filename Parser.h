#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H
#include "Scanner.h"
#include "Symbol.h"
#include "Expression.h"
#include "Block.h"
#include <set>

class Parser {
public:
    Parser(char*);
    void ParseDeclaration(SymbolTable*);
    Block* ParseBlockStart();
    void Print();

    SymbolTable* symTable;
private:
    Expression* ParseExpression(int);
    Expression* ParseTerm(bool);
    Expression* ParseFactor();
    std::vector <Expression*> ParseArrayIndices();
    bool PriorityCheck(int, TokenType);
    void CheckNextLexem(Lexem, Lexem);
    void ParseConstantDeclaration(SymbolTable*);
    void ParseTypeDeclaration(SymbolTable*);
    void ParseVarDeclaration(SymbolTable*);
    void ParseFuncDeclaration(SymbolTable*, DeclarationType);
    Symbol* ParseType(SymbolTable*);
    Expression* ParseInit(SymbolTable*);
    Expression* ParseInitializeList(SymbolTable*);
    Symbol* ParseArrayDecl(SymbolTable*);
    Symbol* ParseRecord(SymbolTable*);
    Block* ParseCompoundBlock(SymbolTable*, int);
    Block* ParseForBlock(SymbolTable*, int);
    Block* ParseBlock(SymbolTable*, int);
    Block* ParseIfBlock(SymbolTable*, int);
    Block* ParseWhileBlock(SymbolTable*, int);
    Block* ParseRepeatBlock(SymbolTable*, int);
    std::vector<Block*> ParseBlockList(SymbolTable*, int);
    Block* ParseBlockIdent(SymbolTable*, int);
    int ParseArguments(SymbolTable*);
    void CheckConstant(SymbolTable*, Expression*);
    std::vector<int> priorities;
    std::vector<int> unarPriorities;
    Scanner scanner;
    Expression* expression;
};

#endif //COMPILER_PARSER_H
