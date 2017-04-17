#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H
#include "Scanner.h"
#include "Symbol.h"
#include "Expression.h"

class Parser {
public:
    Parser(char*);
    void ParseDeclaration(SymbolTable*);
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
    Symbol* ParseType(SymbolTable*);
    Expression* ParseInit(SymbolTable*);
    Expression* ParseInitializeList(SymbolTable*);
    Symbol* ParseArrayDecl(SymbolTable*);
    Symbol* ParseRecord(SymbolTable*);
    int ParseArguments(SymbolTable*);
    void CheckConstant(SymbolTable*, Expression*);
    std::vector<int> priorities;
    std::vector<int> unarPriorities;
    Scanner scanner;
    Expression* expression;
};

#endif //COMPILER_PARSER_H
