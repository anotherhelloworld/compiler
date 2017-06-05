#ifndef COMPILER_EXPRESSION_H
#define COMPILER_EXPRESSION_H
#include "Scanner.h"
//#include "Symbol.h"
#define indent "   "

enum class DataType;
class Symbol;

enum class ExpressionType {
    VAR, BINOP, UNOP, INT, REAL, CHAR, BOOLEAN, IDENT, ARRAY, RECORD, ASSIGN, FUNCCALL,
};

struct ExpressionArgumentList {
    std::vector<std::string> arguments;
    bool flag;
    ExpressionArgumentList(): flag(true) {};
};

class Expression {
public:
    DataType typeID;
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
    Expression* left;
    Expression* right;
    Lexem operation;
protected:
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
    Symbol* symbol;
    ExpressionIdent(const Lexem &val) : ExpressionTerm(val, ExpressionType::IDENT) {};
    ExpressionIdent(const Lexem &val, Symbol* symbol) : symbol(symbol), ExpressionTerm(val, ExpressionType::IDENT) {};
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

class ExpressionFuncCall: public Expression {
public:
    Expression* left;
    std::vector<Expression*> args;
    ExpressionFuncCall(Expression* left, std::vector<Expression*> args): Expression(ExpressionType::FUNCCALL), left(left), args(args) {};
    void Print(int);
};

#endif //COMPILER_EXPRESSION_H
