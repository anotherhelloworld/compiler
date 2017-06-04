#include "TypeChecker.h"
#include "Symbol.h"

DataType CastTable[3][3] = {
        {DataType::INTEGER, DataType::BADTYPE, DataType::BADTYPE},
        {DataType::REAL, DataType::REAL, DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::CHAR}
};

TypeChecker::TypeChecker(SymbolTable *symbolTable, Expression* expr, std::pair<int, int> pos): symbolTable(symbolTable), pos(pos) {
    Check(GetTypeID(((ExpressionAssign*)expr)->left), GetTypeID(((ExpressionAssign*)expr)->right));
}

void TypeChecker::Check(DataType type1, DataType type2) {
    if (!CanCast(type1, type2)) {
        throw "IncompatibleTypes";
    }
}

bool TypeChecker::CanCast(DataType type1, DataType type2) {
    return CastTable[(int)type1][(int)type2] != DataType::BADTYPE;
}

DataType TypeChecker::GetTypeID(Expression* expr) {
    if (expr->expressionType == ExpressionType::IDENT) {
        Symbol* type = (((ExpressionIdent*)expr)->symbol)->GetType();
        if (type == nullptr) {
            return DataType::BADTYPE;
        }
        if (((ExpressionIdent*)expr)->symbol->GetType()->declType == DeclarationType::RECORD) {
            return  DataType::BADTYPE;
        }
        return ((SymbolType*)(((ExpressionIdent*)expr)->symbol)->GetType())->dataType;
    }
    return GetTypeID(expr->expressionType);
}

DataType TypeChecker::GetTypeID(ExpressionType exprType) {
    if (exprType == ExpressionType::INT) {
        return DataType::INTEGER;
    }
    if (exprType == ExpressionType::REAL) {
        return DataType::REAL;
    }
    throw "Illegal exprexxion";
}

