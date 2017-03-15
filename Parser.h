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
    Expression* ParseExpression();
    Expression* ParseNextExpression();
    Expression* ParseTerm();
    Expression* ParseFactor();
    void Print();
private:
    Scanner scanner;
    Expression* expression;
};

class ExpressionBinOp: Expression {
public:
    ExpressionBinOp(Lexem operation, Expression* right, Expression* left): operation(operation), right(right), left(left) {};
    void Print(int);
private:
    Lexem operation;
    Expression* left;
    Expression* right;
};

class ExpressionUnOp: Expression {
public:
    ExpressionUnOp(Lexem operation, Expression* arg): operation(operation), arg(arg) {};
    void Print(int);
private:
    Lexem operation;
    Expression* arg;
};

class ExpressionInteger: Expression {
public:
    ExpressionInteger(Lexem val): val(val) {};
    void Print(int);
private:
    Lexem val;
};

class ExpressionReal: Expression {
public:
    ExpressionReal(Lexem val): val(val) {};
    void Print(int);
private:
    Lexem val;
};

class ExpressionIdent: Expression {
public:
    ExpressionIdent(Lexem val): val(val) {};
    void Print(int);
private:
    Lexem val;
};


#endif //COMPILER_PARSER_H
