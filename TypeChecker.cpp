#include "TypeChecker.h"
#include "Symbol.h"

std::map <DataType, std::map<TokenType, DataType> > operationsTypes = {
        { DataType::INTEGER,
                {
                        { EQUAL,     DataType::BOOLEAN },
                        { XOR,       DataType::INTEGER },
                        { ADD,       DataType::INTEGER },
                        { SUB,       DataType::INTEGER },
                        { DIVISION,  DataType::REAL    },
                        { DIV,       DataType::INTEGER },
                        { MULT,      DataType::INTEGER },
                        { MOD,          DataType::INTEGER },
                        { NOT_EQUAL,    DataType::BOOLEAN },
                        { GREATER_THAN, DataType::BOOLEAN },
                        { GREATER_OR_EQUAL_THAN, DataType::BOOLEAN },
                        { LESS_THAN, DataType::BOOLEAN },
                        { LESS_OR_EQUAL_THAN, DataType::BOOLEAN },
                        { SHL, DataType::INTEGER },
                        { SHR, DataType::INTEGER },
                        { AND, DataType::INTEGER },
                        { OR, DataType::INTEGER }

                }
        },
        { DataType::REAL,
                {
                        { EQUAL, DataType::BOOLEAN },
                        { ADD, DataType::REAL },
                        { DIVISION, DataType::REAL },
                        { MULT, DataType::REAL },
                        { NOT_EQUAL, DataType::REAL },
                        { GREATER_THAN, DataType::BOOLEAN },
                        { LESS_THAN, DataType::BOOLEAN },
                        { GREATER_OR_EQUAL_THAN, DataType::BOOLEAN },
                        { LESS_OR_EQUAL_THAN, DataType::BOOLEAN },
                        { SUB, DataType::REAL }
                }
        },
//        { DataType::CHAR,
//                {
//
//                }
//        },
        { DataType::BOOLEAN,
                {
                    { XOR, DataType::BOOLEAN },
                    { AND, DataType::BOOLEAN },
                    { OR, DataType::BOOLEAN }
                }
        },
        { DataType::POINTER,
            {
                {SUB, DataType::INTEGER}
            }
        }
};

DataType CastTable[8][8] = {
        {DataType::INTEGER, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE,  DataType::BADTYPE},
        {DataType::REAL,    DataType::REAL,    DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE,  DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::CHAR,    DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE,  DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BOOLEAN, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE,  DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::ARRAY,   DataType::BADTYPE, DataType::BADTYPE,  DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::STRING,  DataType::BADTYPE, DataType::BADTYPE, DataType::STRING,  DataType::BADTYPE,  DataType::BADTYPE}, 
        {DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::RECORD,   DataType::BADTYPE},
        {DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE, DataType::BADTYPE,  DataType::POINTER}
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
    if (((SymbolType*)(symbol))->dataType == DataType::CHAR) {
        Check(((SymbolType*)(symbol))->dataType, GetTypeID(expr));
        return;
    }
    if (((SymbolType*)(symbol))->dataType == DataType::ARRAY) {
        int left = ((SymbolArray*)symbol)->left;
        int right = ((SymbolArray*)symbol)->right;
        int tmp = right - left + 1;
        int tmp2 = ((ExpressionInitializeList*)expr)->initList.size();
        if (expr->expressionType != ExpressionType::INITLIST || tmp2 != tmp) {
            throw TypeCheckerException(dataTypeString[(int)DataType::ARRAY], pos);
        }
        for (int i = 0; i < ((ExpressionInitializeList*)expr)->initList.size(); i++) {
            TypeChecker(symbolTable, symbol->GetType(), ((ExpressionInitializeList*)expr)->initList[i], pos);
        }
        return;
    }
}

TypeChecker::TypeChecker(SymbolTable* table, Expression* left, Expression* right, std::pair<int, int> pos): symbolTable(table), pos(pos) {
    Check(GetTypeID(left), GetTypeID(right));
}

void TypeChecker::Check(DataType type1, DataType type2) {
    if (!CanCast(type1, type2)) {
//        throw TypeCheckerException(dataTypeString[(int)type1], pos);
//        throw IncompatibleTypes(dataTypeString[(int)type1], dataTypeString[(int)type2], pos);
        this->Error(type1, type2);
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
        Expression* aiExp = expr;
        Symbol* sym = nullptr;
        auto arrayExp = ((ExpressionArrayIndecies*)aiExp)->ident;
        auto temp = arrayExp->expressionType;
        int count = 1;
        while (temp != ExpressionType::VAR) {
            count++;
            arrayExp = ((ExpressionArrayIndecies*)arrayExp)->ident;
            temp = arrayExp->expressionType;
        }
        for (int i = 0; i < count; i++) {
            if (arrayExp->expressionType == ExpressionType::VAR) {
                sym = ((ExpressionIdent*)(arrayExp))->symbol;
            } else if (arrayExp->expressionType == ExpressionType::RECORD) {
                sym = ((ExpressionIdent*)(((ExpressionRecordAccess*)(((ExpressionArrayIndecies*)aiExp)->ident))->right))->symbol;
            }
        }
        for (int i = 0; i <= count; i++) {
            if (sym->declType == DeclarationType::TYPE) {
                sym = sym->GetType();
            } else if (sym->declType == DeclarationType::VAR || sym->declType == DeclarationType::CONST) {
                sym = sym->GetType();
            } else if (sym->declType == DeclarationType::FUNC) {
                sym = sym->GetType();
            }
        }
        return ((SymbolType*)sym)->dataType;
    }
    if (expr->expressionType == ExpressionType::VAR) {
        Symbol* type = (((ExpressionIdent*)expr)->symbol)->GetType();
        if (type == nullptr) {
            return DataType::BADTYPE;
        }
        if (((ExpressionIdent*)expr)->symbol->GetType()->declType == DeclarationType::RECORD) {
            return  DataType::BADTYPE;
        }
        return ((SymbolType*)(((ExpressionIdent*)expr)->symbol)->GetType())->dataType;
    }
    if (expr->expressionType == ExpressionType::FUNCCALL) {
        auto symbol = symbolTable->FindReqSymbol(expr, pos);
        if (symbol->declType == DeclarationType::PROCEDURE) {
            return DataType::BADTYPE;
        } else {
            if (symbol->GetType()->declType == DeclarationType::RECORD) {
                return DataType::RECORD;
            }
            return ((SymbolType*)symbol->GetType())->dataType;
        }
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
    if (exprType == ExpressionType::BOOLEAN) {
        return DataType ::BOOLEAN;
    }
    throw ParserException("Illegal expression");
}

DataType TypeChecker::GetTypeIDBinExpression(DataType left, DataType right, TokenType token) {
    if (CanCast(left, right)) {
        right = left;
    } else if (CanCast(right, left)) {
        left = right;
    }
    else {
//        throw TypeCheckerException(dataTypeString[(int)left], pos);
//        throw IncompatibleTypes(dataTypeString[(int)right], dataTypeString[(int)left], pos);
        this->Error(right, left);
    }
    try {
        DataType res = operationsTypes.at(left).at(token);
        return res;
    } catch (std::out_of_range error) {
        return DataType::BADTYPE;
    }
}

bool CmpArguments::CompareTypes(Symbol* type1, Symbol* type2) {
    if (((SymbolType*)type1)->dataType != ((SymbolType*)type2)->dataType) {
        return false;
    }
    bool ans;
    if (((SymbolType*)type1)->dataType == DataType::BOOLEAN) {
        return true;
    }
    if (((SymbolType*)type1)->dataType == DataType::CHAR) {
        return true;
    }
    if (((SymbolType*)type1)->dataType == DataType::REAL) {
        return true;
    }
    if (((SymbolType*)type1)->dataType == DataType::INTEGER) {
        return true;
    }
    if (((SymbolType*)type1)->dataType == DataType::ARRAY) {
        return Compare(type1->GetType(), type2->GetType());
    }
    if (((SymbolType*)type1)->dataType == DataType::RECORD) {
        ans = true;
        if (((SymbolRecord*)type1->GetType())->argc != ((SymbolRecord*)(type2->GetType()))->argc) {
            return false;
        }
        for (int i = 0; i < ((SymbolRecord*)type1->GetType())->table->symbols.size(); i++) {
            ans = ans && Compare(((SymbolRecord*)type1->GetType())->table->symbols[i], ((SymbolRecord*)type2->GetType())->table->symbols[i]);
        }
        return ans;
    }
    return false;
}

void TypeChecker::Error(DataType type1, DataType type2) {
    std::string strType1 = (int)type1 == -1 ? "BadType" : dataTypeString[(int)type1];
    std::string strType2 = (int)type2 == -1 ? "BadType" : dataTypeString[(int)type2];
    throw IncompatibleTypes(strType1, strType2, pos);
}

bool CmpArguments::Compare(Symbol* sym1, Symbol* sym2) {
    auto symR1 = (SymbolFunction*)sym1;
    auto symR2 = (SymbolFunction*)sym2;
    if (symR1->argc != symR2->argc) {
        return false;
    }
    for (int i = 0; i < symR1->argc; i++) {
        if (symR1->symbolTable->symbols[i]->GetType() != symR2->symbolTable->symbols[i]->GetType()) {
            return false;
        }
    }
    return true;
}