#ifndef COMPILER_EXPRESSION_H
#define COMPILER_EXPRESSION_H
#include "Scanner.h"

enum class ExpressionType {
    VAR, BINOP, UNOP, INT, REAL, CHAR, BOOLEAN, IDENT, ARRAY, RECORD, ASSIGN,
};

struct ExpressionArgumentList {
    std::vector<std::string> arguments;
    bool flag;
    ExpressionArgumentList(): flag(true) {};
};

class Expression {
public:
    ExpressionType expressionType;
    Expression(ExpressionType expressionType): expressionType(expressionType) {};
    virtual void Print(int) {};
    virtual void GetIdentificitationList(ExpressionArgumentList*) {};
};

class ExpressionBinOp: public Expression {
public:
    ExpressionBinOp(Lexem operation, Expression* right, Expression* left): Expression(ExpressionType::BINOP), operation(operation), right(right), left(left) {};
    void Print(int);
    void GetIdentificitationList(ExpressionArgumentList*);
protected:
    Lexem operation;
    Expression* left;
    Expression* right;
};

class ExpressionTerm: Expression {
public:
    ExpressionTerm(const Lexem &val, ExpressionType expressionType): Expression(expressionType), val(val) {};
    void Print(int);
    void GetIdentificitationList(ExpressionArgumentList*);
protected:
    Lexem val;
};

class ExpressionUnOp: Expression {
public:
    ExpressionUnOp(Lexem operation, Expression* arg): Expression(ExpressionType::UNOP), operation(operation), arg(arg) {};
    void Print(int);
    void GetIdentificitationList(ExpressionArgumentList*);
private:
    Lexem operation;
    Expression* arg;
};

class ExpressionInteger: ExpressionTerm {
public:
    ExpressionInteger(const Lexem &val): ExpressionTerm(val, ExpressionType::INT) {};
};

class ExpressionReal: ExpressionTerm {
public:
    ExpressionReal(const Lexem &val): ExpressionTerm(val, ExpressionType::REAL) {};
};

class ExpressionChar: ExpressionTerm {
public:
    ExpressionChar(const Lexem &val): ExpressionTerm(val, ExpressionType::CHAR) {};
};

class ExpressionBoolean: ExpressionTerm {
public:
    ExpressionBoolean(const Lexem &val): ExpressionTerm(val, ExpressionType::BOOLEAN) {};
};

class ExpressionIdent: ExpressionTerm {
public:
    ExpressionIdent(const Lexem &val) : ExpressionTerm(val, ExpressionType::IDENT) {};
};

class ExpressionRecordAccess: ExpressionBinOp {
public:
    ExpressionRecordAccess(Expression* right, Expression* left): ExpressionBinOp(Lexem(".", POINT), right, left) {}
};

class ExpressionArrayIndecies: Expression {
public:
    ExpressionArrayIndecies(Expression* ident, std::vector<Expression*> indecies):
            Expression(ExpressionType::ARRAY), operation(Lexem("[]", OPEN_SQUARE_BRACKET)), ident(ident), indecies(indecies) {};
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
    ExpressionInitializeList(std::vector<Expression*> initList = std::vector<Expression*>()):
            Expression(ExpressionType::VAR), initList(initList) {};
    void Print(int);
};

class ExpressionAssign: public Expression {
public:
    Expression* left;
    Expression* right;
    ExpressionAssign(Expression* left, Expression* right): Expression(ExpressionType::ASSIGN), left(left), right(right) {};
    void Print(int);
};

#endif //COMPILER_EXPRESSION_H
