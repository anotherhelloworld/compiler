#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H
#include "Scanner.h"

class Expression {
public:
    Expression() {};
    virtual void Print(int) {};
};

class Parser {
public:
    Parser(char*);
    Expression* ParseExpression(int);
    Expression* ParseTerm(bool);
    Expression* ParseFactor();
    std::vector <Expression*> ParseArrayIndices();
    void Print();
    bool PriorityCheck(int, TokenType);
    void CheckNextLexem(Lexem, Lexem);
    std::vector<int> priorities;
    std::vector<int> unarPriorities;
private:
    Scanner scanner;
    Expression* expression;

};

class ExpressionBinOp: public Expression {
public:
    ExpressionBinOp(Lexem operation, Expression* right, Expression* left): operation(operation), right(right), left(left) {};
    void Print(int);
protected:
    Lexem operation;
    Expression* left;
    Expression* right;
};

class ExpressionTerm: Expression {
public:
    ExpressionTerm(const Lexem &val) : val(val) {};
    void Print(int);
protected:
    Lexem val;
};

class ExpressionUnOp: Expression {
public:
    ExpressionUnOp(Lexem operation, Expression* arg): operation(operation), arg(arg) {};
    void Print(int);
private:
    Lexem operation;
    Expression* arg;
};

class ExpressionInteger: ExpressionTerm {
public:
    ExpressionInteger(const Lexem &val) : ExpressionTerm(val) {};
};

class ExpressionReal: ExpressionTerm {
public:
    ExpressionReal(const Lexem &val) : ExpressionTerm(val) {};
};

class ExpressionIdent: ExpressionTerm {
public:
    ExpressionIdent(const Lexem &val) : ExpressionTerm(val) {};
};

class ExpressionRecordAccess: ExpressionBinOp {
public:
    ExpressionRecordAccess(Expression* right, Expression* left): ExpressionBinOp(Lexem(".", POINT), right, left) {}
};

class ExpressionArrayIndecies: Expression {
public:
    ExpressionArrayIndecies(Expression* ident, std::vector<Expression*> indecies):
            operation(Lexem("[]", OPEN_SQUARE_BRACKET)), ident(ident), indecies(indecies) {};
    void Print(int);
private:
    Lexem operation;
    Expression* ident;
    std::vector<Expression*> indecies;
};

#endif //COMPILER_PARSER_H
