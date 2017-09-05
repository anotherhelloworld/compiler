#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H
#include "Scanner.h"
#include "Symbol.h"
#include "Expression.h"
#include "Block.h"
#include "TypeChecker.h"
#include <set>

typedef struct declCall {
    Symbol* sym;
    std::pair <int, int> pos;
} declCall;

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
    Expression* ParseInit(SymbolTable*);
    Expression* ParseInitializeList(SymbolTable*);

    std::vector <Expression*> ParseArrayIndices(SymbolTable*);

    bool PriorityCheck(int, TokenType);
    void CheckNextLexem(Lexem, Lexem);
    void ParseConstantDeclaration(SymbolTable*);
    void ParseTypeDeclaration(SymbolTable*);
    void ParseVarDeclaration(SymbolTable*);
    void ParseFuncDeclaration(SymbolTable*, DeclarationType);
    void ParseLabelDeclaration(SymbolTable*);

    Symbol* ParseType(SymbolTable*);    
    Symbol* ParseArrayDecl(SymbolTable*);
    Symbol* ParseRecord(SymbolTable*);
    Symbol* ParseString(SymbolTable*);

    Block* ParseCompoundBlock(SymbolTable*, int);
    Block* ParseForBlock(SymbolTable*, int);
    Block* ParseBlock(SymbolTable*, int);
    Block* ParseIfBlock(SymbolTable*, int);
    Block* ParseWhileBlock(SymbolTable*, int);
    Block* ParseRepeatBlock(SymbolTable*, int);
    Block* ParseBlockIdent(SymbolTable*, int);
    Block* ParseGoToBlock(SymbolTable*, int);
    Block* ParseTryBlock(SymbolTable*, int);

    std::vector<Block*> ParseBlockList(SymbolTable*, int);

    int ParseArguments(SymbolTable*);
    void CheckConstant(SymbolTable*, Expression*);
    std::vector<int> priorities;
    std::vector<int> unarPriorities;
    Scanner scanner;
    Expression* expression;
    std::vector <declCall> declForwardCall;
};

#endif //COMPILER_PARSER_H
