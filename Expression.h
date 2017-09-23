#ifndef COMPILER_EXPRESSION_H
#define COMPILER_EXPRESSION_H
#include <set>
#include <algorithm>
#include <cmath>
#include "Scanner.h"
#include "Generator.h"
#define indent "   "

#define WRITE -1
#define WRITELN -2

void printIndent(int spaces);

enum class DataType;
class Symbol;

enum class ExpressionType {
    VAR, BINOP, UNOP, INT, REAL, CHAR, BOOLEAN, IDENT, ARRAY, RECORD, ASSIGN, FUNCCALL, INITLIST, POINTER, DEREFERENCE
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
    virtual void Generate(Generator* generator) {};
    virtual int GetSize() { return 0; };
};

class ExpressionBinOp: public Expression {
public:
    ExpressionBinOp(Lexem operation, Expression* right, Expression* left): Expression(ExpressionType::BINOP), operation(operation), right(right), left(left) {};
    void Print(int);
    void GetIdentificitationList(ExpressionArgumentList*);
    void Generate(Generator*);
    int GetSize();
    void GenerateBoolExpr(Generator*, AsmTypeOperation);
    Expression* left;
    Expression* right;
    Lexem operation;
protected:
};

class ExpressionTerm: public Expression {
public:
    ExpressionTerm(const Lexem &val, ExpressionType expressionType): Expression(expressionType), val(val) {};
    void Print(int);
    void GetIdentificitationList(ExpressionArgumentList*);
    Lexem val;
protected:
};

class ExpressionUnOp: Expression {
public:
    ExpressionUnOp(Lexem operation, Expression* arg): Expression(ExpressionType::UNOP), operation(operation), arg(arg) {};
    void Print(int);
    void GetIdentificitationList(ExpressionArgumentList*);
    Lexem operation;
    Expression* arg;

};

class ExpressionInteger: public ExpressionTerm {
public:
    ExpressionInteger(const Lexem &val): ExpressionTerm(val, ExpressionType::INT) {};
    void Generate(Generator*);
    int GetSize();
};

class ExpressionReal: ExpressionTerm {
public:
    ExpressionReal(const Lexem &val): ExpressionTerm(val, ExpressionType::REAL) {};
    void Generate(Generator*);
    int GetSize();
};

class ExpressionChar: public ExpressionTerm {
public:
    ExpressionChar(const Lexem &val): ExpressionTerm(val, ExpressionType::CHAR) {};
    void Generate(Generator*);
    int GetSize();
};

class ExpressionBoolean: ExpressionTerm {
public:
    ExpressionBoolean(const Lexem &val): ExpressionTerm(val, ExpressionType::BOOLEAN) {};
};

class ExpressionIdent: public ExpressionTerm {
public:
    Symbol* symbol;
    ExpressionIdent(const Lexem &val) : ExpressionTerm(val, ExpressionType::VAR) {};
    ExpressionIdent(const Lexem &val, Symbol* symbol) : symbol(symbol), ExpressionTerm(val, ExpressionType::VAR) {};
};

class ExpressionRecordAccess: public ExpressionBinOp {
public:
    Symbol* field;
    ExpressionRecordAccess(Expression* right, Expression* left, Symbol* field): ExpressionBinOp(Lexem(".", POINT), right, left), field(field) {
        expressionType = ExpressionType::RECORD;
    };
};

class ExpressionArrayIndecies: public Expression {
public:
    ExpressionArrayIndecies(Expression* ident, std::vector<Expression*> indecies):
            Expression(ExpressionType::ARRAY), operation(Lexem("[]", OPEN_SQUARE_BRACKET)), ident(ident), indecies(indecies) {};
    void Print(int);
    void GetIdentificitationList(ExpressionArgumentList*);
    std::vector<Expression*> indecies;
    Lexem operation;
    Expression* ident;
};

class ExpressionInitializeList: public Expression {
public:
    std::vector<Expression*> initList;
    ExpressionInitializeList(std::vector<Expression*> initList = std::vector<Expression*>()):
            Expression(ExpressionType::INITLIST), initList(initList) {};
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
    void Generate(Generator* generator);
    void GenerateWrite(Generator* generator, int argc);
    void Print(int);
};

class ExpressionPointer: public Expression {
public:
    Expression* exp;
    ExpressionPointer(Expression* exp): exp(exp), Expression(ExpressionType::POINTER) {};
    void Print(int);
};

class ExpressionDereference: public Expression {
public:
    Expression* exp;
    ExpressionDereference(Expression* exp): exp(exp), Expression(ExpressionType::DEREFERENCE) {};
    void Print(int);
};

#endif //COMPILER_EXPRESSION_H
