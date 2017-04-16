#ifndef COMPILER_EXPRESSION_H
#define COMPILER_EXPRESSION_H
#include "Scanner.h"

struct ExpressionArgumentList {
    std::vector<std::string> arguments;
    bool flag;
    ExpressionArgumentList(): flag(true) {};
};

class Expression {
public:
    Expression() {};
    virtual void Print(int) {};
    virtual void GetIdentificitationList(ExpressionArgumentList*) {};
};

class ExpressionBinOp: public Expression {
public:
    ExpressionBinOp(Lexem operation, Expression* right, Expression* left): operation(operation), right(right), left(left) {};
    void Print(int);
    void GetIdentificitationList(ExpressionArgumentList*);
protected:
    Lexem operation;
    Expression* left;
    Expression* right;
};

class ExpressionTerm: Expression {
public:
    ExpressionTerm(const Lexem &val): val(val) {};
    void Print(int);
    void GetIdentificitationList(ExpressionArgumentList*);
protected:
    Lexem val;
};

class ExpressionUnOp: Expression {
public:
    ExpressionUnOp(Lexem operation, Expression* arg): operation(operation), arg(arg) {};
    void Print(int);
    void GetIdentificitationList(ExpressionArgumentList*);
private:
    Lexem operation;
    Expression* arg;
};

class ExpressionInteger: ExpressionTerm {
public:
    ExpressionInteger(const Lexem &val): ExpressionTerm(val) {};
};

class ExpressionReal: ExpressionTerm {
public:
    ExpressionReal(const Lexem &val): ExpressionTerm(val) {};
};

class ExpressionChar: ExpressionTerm {
public:
    ExpressionChar(const Lexem &val): ExpressionTerm(val) {};
};

class ExpressionBoolean: ExpressionTerm {
public:
    ExpressionBoolean(const Lexem &val): ExpressionTerm(val) {};
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
    void GetIdentificitationList(ExpressionArgumentList*);
private:
    Lexem operation;
    Expression* ident;
    std::vector<Expression*> indecies;
};

class ExpressionInitializeList: public Expression {
public:
    std::vector<Expression*> initList;
    ExpressionInitializeList(std::vector<Expression*> initList = std::vector<Expression*>()): initList(initList) {};
    void Print(int);
};

#endif //COMPILER_EXPRESSION_H
