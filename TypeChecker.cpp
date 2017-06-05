#include "TypeChecker.h"
#include "Symbol.h"

std::map <DataType, std::map<TokenType, DataType> > operationsTypes = {
        { DataType::INTEGER,
                {
                        { EQUAL, DataType::BOOLEAN },
                        { XOR, DataType::INTEGER },
                        { ADD, DataType::INTEGER },
                        { DIVISION, DataType::REAL },
                        { DIV, DataType::INTEGER }
                }
        },
        { DataType::REAL,
                {
                        { EQUAL, DataType::BOOLEAN },
                        { ADD, DataType::REAL },
                        { DIVISION, DataType::REAL }
                }
        }
};

DataType CastTable[4][4] = {
        {DataType::INTEGER, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE},
        {DataType::REAL, DataType::REAL, DataType::BADTYPE, DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::CHAR, DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BOOLEAN}
};

TypeChecker::TypeChecker(SymbolTable* symbolTable, Expression* expr, std::pair<int, int> pos): symbolTable(symbolTable), pos(pos) {
    Check(GetTypeID(((ExpressionAssign*)expr)->right), GetTypeID(((ExpressionAssign*)expr)->left));
}

TypeChecker::TypeChecker(SymbolTable* symbolTable, DataType type1, Expression* expr, std::pair<int, int> pos): symbolTable(symbolTable), pos(pos) {
    Check(type1, GetTypeID(expr));
}

TypeChecker::TypeChecker(SymbolTable* symbolTable, Symbol* symbol, Expression* expr, std::pair<int, int> pos): symbolTable(symbolTable), pos(pos) {
    if (((SymbolType*)(symbol))->dataType == DataType::INTEGER) {
        Check(((SymbolType*)(symbol))->dataType, GetTypeID(expr));
        return;
    }
    if (((SymbolType*)(symbol))->dataType == DataType::REAL) {
        Check(((SymbolType*)(symbol))->dataType, GetTypeID(expr));
        return;
    }
}

void TypeChecker::Check(DataType type1, DataType type2) {
    if (!CanCast(type1, type2)) {
        throw TypeCheckerException("IncompatibleTypes");
    }
}

bool TypeChecker::CanCast(DataType type1, DataType type2) {
    return CastTable[(int)type1][(int)type2] != DataType::BADTYPE;
}

DataType TypeChecker::GetTypeID(Expression* expr) {
    if (expr->expressionType == ExpressionType::BINOP) {
        return GetTypeIDBinExpression(GetTypeID(((ExpressionBinOp*)expr)->left), GetTypeID(((ExpressionBinOp*)expr)->right), ((ExpressionBinOp*)expr)->operation.token);
    }
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
    throw ParserException("Illegal exprexxion");
}

DataType TypeChecker::GetTypeIDBinExpression(DataType left, DataType right, TokenType token) {
    if (CanCast(left, right)) {
        right = left;
    } else if (CanCast(right, left)) {
        left = right;
    }
    else {
        throw TypeCheckerException("IncompatibleTypes");
    }
    try {
        DataType res = operationsTypes.at(left).at(token);
        return res;
    } catch (std::out_of_range error) {
        return DataType::BADTYPE;
    }
}

