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

DataType CastTable[5][5] = {
        {DataType::INTEGER, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE},
        {DataType::REAL,    DataType::REAL,    DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::CHAR,    DataType::BADTYPE, DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BOOLEAN, DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::ARRAY}
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
    if (((SymbolType*)(symbol))->dataType == DataType::CHAR) {
        Check(((SymbolType*)(symbol))->dataType, GetTypeID(expr));
        return;
    }
    if (((SymbolType*)(symbol))->dataType == DataType::ARRAY) {
        if (expr->expressionType != ExpressionType::INITLIST || ((ExpressionInitializeList*)expr)->initList.size()
                                                                != atoi(((ExpressionTerm*)(((SymbolArray*)symbol)->right))->val.val.c_str())
                                                                   - atoi(((ExpressionTerm*)(((SymbolArray*)symbol)->left))->val.val.c_str()) + 1) {
            throw TypeCheckerException(dataTypeString[(int)DataType::ARRAY], dataTypeString[(int)GetTypeID(expr->expressionType)], pos);
        }
        for (int i = 0; i < ((ExpressionInitializeList*)expr)->initList.size(); i++) {
            TypeChecker(symbolTable, symbol->GetType(), ((ExpressionInitializeList*)expr)->initList[i], pos);
        }
        return;
    }
}

void TypeChecker::Check(DataType type1, DataType type2) {
    if (!CanCast(type1, type2)) {
        throw TypeCheckerException(dataTypeString[(int)type1], dataTypeString[(int)type2], pos);
    }
}

bool TypeChecker::CanCast(DataType type1, DataType type2) {
    return CastTable[(int)type1][(int)type2] != DataType::BADTYPE;
}

DataType TypeChecker::GetTypeID(Expression* expr) {
    if (expr->expressionType == ExpressionType::BINOP) {
        return GetTypeIDBinExpression(GetTypeID(((ExpressionBinOp*)expr)->left), GetTypeID(((ExpressionBinOp*)expr)->right), ((ExpressionBinOp*)expr)->operation.token);
    }
    if (expr->expressionType == ExpressionType::ARRAY) {


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
    if (exprType == ExpressionType::CHAR) {
        return DataType::CHAR;
    }
    if (exprType == ExpressionType::ARRAY) {
        return DataType::ARRAY;
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
        throw TypeCheckerException(dataTypeString[(int)left], dataTypeString[(int)right], pos);
    }
    try {
        DataType res = operationsTypes.at(left).at(token);
        return res;
    } catch (std::out_of_range error) {
        return DataType::BADTYPE;
    }
}

