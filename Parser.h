#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H
#include "Scanner.h"
#include "Symbol.h"
#include "Expression.h"
#include "Block.h"
#include "TypeChecker.h"
#include <set>

class Parser {
public:
    Parser(char*);
    void ParseDeclaration(SymbolTable*);
    Block* ParseBlockStart();
    void Print();
    bool testType = false;
    bool testDeclarations = false;
    SymbolTable* symTable;
private:
    Expression* ParseExpression(SymbolTable*, int);
    Expression* ParseTerm(SymbolTable*, bool);
    Expression* ParseFactor(SymbolTable*);
    std::vector <Expression*> ParseArrayIndices(SymbolTable*);
    bool PriorityCheck(int, TokenType);
    void CheckNextLexem(Lexem, Lexem);
    void ParseConstantDeclaration(SymbolTable*);
    void ParseTypeDeclaration(SymbolTable*);
    void ParseVarDeclaration(SymbolTable*);
    void ParseFuncDeclaration(SymbolTable*, DeclarationType);
    void ParseLabelDeclaration(SymbolTable*);
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
