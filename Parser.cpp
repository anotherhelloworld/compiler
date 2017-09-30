#include "Parser.h"
#include "CalculateExpression.h"

void Parser::Print() {
    expression = ParseExpression(symTable, 0);
    scanner.CheckCurLexem(T_EOF, "EOF");
//    if (scanner.GetLexem().token != TokenType::T_EOF)
//        throw ParserException("Illegal expression in pos " + scanner.GetLexem().GetStrPos());

    expression->Print(0);
}

Parser::Parser(char* filename): scanner(filename) {
    priorities = std::vector<int> (COUNT, -1);
    unarPriorities = std::vector<int> (COUNT, -1);
    symTable = new SymbolTable(nullptr);

    scanner.NextToken();
    priorities[EQUAL] = 0;
    priorities[NOT_EQUAL] = 0;
    priorities[GREATER_THAN] = 0;
    priorities[LESS_THAN] = 0;
    priorities[LESS_OR_EQUAL_THAN] = 0;
    priorities[GREATER_OR_EQUAL_THAN] = 0;
    priorities[ASSIGNMENT] = 0;

    priorities[ADD] = 1;
    priorities[SUB] = 1;
    priorities[OR] = 1;
    priorities[XOR] = 1;


    priorities[MULT] = 2;
    priorities[DIV] = 2;
    priorities[DIVISION] = 2;
    priorities[MOD] = 2;
    priorities[AND] = 2;
    priorities[SHR] = 2;
    priorities[SHL] = 2;

    unarPriorities[NOT] = 3;
    unarPriorities[SUB] = 3;
    unarPriorities[ADD] = 3;
    unarPriorities[DOG] = 3;
    unarPriorities[CIRCUMFLEX] = 3;

    symTable->symbols.push_back(new SymbolType("integer", DataType::INTEGER));
    symTable->symbols.push_back(new SymbolType("real", DataType::REAL));
    symTable->symbols.push_back(new SymbolType("char", DataType::CHAR));
    symTable->symbols.push_back(new SymbolType("boolean", DataType::BOOLEAN));
    symTable->symbols.push_back(new SymbolType("array", DataType::ARRAY));
    symTable->symbols.push_back(new SymbolType("string", DataType::STRING));
    symTable->symbols.push_back(new SymbolType("record", DataType::RECORD));
    symTable->symbols.push_back(new SymbolType("pointer", DataType::POINTER));
    symTable->stdTypeCount = symTable->symbols.size();

    ReserveCastFunc(symTable);


//    expression = ParseExpression(0);
//    if (scanner.GetLexem().token != TokenType::T_EOF)
//        throw ParserException("Illegal expression in pos " + scanner.GetLexem().GetStrPos());
}

void Parser::CastFunction(SymbolTable* table, std::string type1, std::string type2) {
    auto newTable = new SymbolTable(table);
    Symbol* symArg = new SymbolVar("arg0", nullptr, table->GetSymbol(type2, std::make_pair(0, 0)), ArgTypeState::RVALUE);
    newTable->Add(symArg);
    Symbol* symRes = new SymbolVar("result", nullptr, table->GetSymbol(type1, std::make_pair(0, 0)), ArgTypeState::RVALUE);
    newTable->Add(symRes);
    auto block = new BlockCompound();
    block->Add(new BlockAssign(new ExpressionAssign(new ExpressionIdent(Lexem("result", START, std::make_pair(0, 0)), newTable->GetSymbol("result", std::make_pair(0, 0))),
        new ExpressionIdent(Lexem("arg0", START, std::make_pair(0, 0)), newTable->GetSymbol("arg0", std::make_pair(0, 0))))));
    auto symFunc = new SymbolFunction(type1, newTable, new BlockCompound(), 2, table->GetSymbol(type1, std::make_pair(0, 0)));
    table->Add(symFunc);
    table->stdTypeCount++;
}

void Parser::ReserveCastFunc(SymbolTable* table) {
    CastFunction(table, "real", "integer");
    CastFunction(table, "integer", "char");
    CastFunction(table, "char", "integer");
    table->Add(new SymbolProcedure("write", new SymbolTable(table), nullptr, WRITE));
    table->Add(new SymbolProcedure("writeln", new SymbolTable(table), nullptr, WRITELN));
    table->stdTypeCount++;
    table->stdTypeCount++;
}

Expression* Parser::ParseExpression(SymbolTable* table, int priority) {
    if (priority == 3) {
        return ParseFactor(table);
    }
    Expression* res = ParseExpression(table, priority + 1);
    if (testType) {
        res->typeID = TypeChecker(table, scanner.GetLexem().pos).GetTypeID(res);
    }
    Lexem lex = scanner.GetLexem();
    while (PriorityCheck(priority, lex.token)) {
        if (scanner.GetLexem().token == ASSIGNMENT) {
            scanner.NextToken();
            Expression* right = ParseExpression(table, priority + 1);
            res = (Expression*)new ExpressionAssign(right, res);
            if (testType) {
                TypeChecker(table, res, scanner.GetLexem().pos);
            }
            lex = scanner.GetLexem();
            return res;
        }
        scanner.NextToken();
        Expression* right = ParseExpression(table, priority + 1);
//        right->typeID = TypeChecker(table, scanner.GetLexem().pos).GetTypeID(right);
        res = (Expression*)new ExpressionBinOp(lex, res, right);
        if (testType) {
            res->typeID = TypeChecker(table, scanner.GetLexem().pos).GetTypeID(res);
            right->typeID = TypeChecker(table, scanner.GetLexem().pos).GetTypeID(right);
        }
        lex = scanner.GetLexem();
    }
    return res;
}

Expression* Parser::ParseFactor(SymbolTable* table) {
    auto lex = scanner.GetLexem();
    if (PriorityCheck(3, lex.token)) {
        scanner.NextToken();
        auto curExp = ParseExpression(table, 0);
        if (lex.token == DOG) {
            return (Expression*)new ExpressionPointer(curExp);
        }
        return (Expression*)new ExpressionUnOp(lex, curExp);
    }
    if (lex.token == OPEN_BRACKET) {
        scanner.NextToken();
        auto curExp = ParseExpression(table, 0);
        scanner.CheckCurLexem(CLOSE_BRACKET, ")");
        scanner.NextToken();
        return curExp;
    }
    if (lex.token == NUMBER) {
        scanner.NextToken();
        return (Expression*)new ExpressionInteger(lex);
    }
    if (lex.token == REAL_NUMBER) {
        scanner.NextToken();
        return (Expression*)new ExpressionReal(lex);
    }
    if (lex.token == TOKEN_STRING) {
        scanner.NextToken();
        return (Expression*)new ExpressionChar(lex);
    }
    if (lex.token == TRUE || lex.token == FALSE) {
        scanner.NextToken();
        return (Expression*)new ExpressionBoolean(lex);
    }
    if (lex.token == IDENTIFIER) {
        return ParseTerm(table, true);
    }
//    throw ParserException("Illegal expression in pos " + lex.GetStrPos());
    throw IllegalExpression(lex.pos);
}

bool Parser::PriorityCheck(int priority, TokenType token) {
    return (priorities[token] == priority || unarPriorities[token] == priority);
}

Expression *Parser::ParseTerm(SymbolTable* table, bool flag) {
    Lexem lex = scanner.GetLexem();
    Expression* res = nullptr;
    Symbol* sym = nullptr;
    if (testType || testDeclarations) {
        sym = table->GetSymbol(lex.val, lex.pos);
        res = (Expression*)new ExpressionIdent(lex, sym);
    } else {
        res = (Expression*)new ExpressionIdent(lex);
    }
    Expression* res2 = nullptr;
    while (flag) {
        scanner.NextToken();
        lex = scanner.GetLexem();
        if (lex.token == POINT) {
            scanner.CheckNextLexem(IDENTIFIER, "IDENTIFIER");
            Expression* right = ParseTerm(table, false);
            if (testType) {
                Symbol* field = nullptr;
                field = ((SymbolRecord*)sym->GetType())->table->GetSymbol(scanner.GetLexem().val, scanner.GetLexem().pos);
                sym = field;
                res = (Expression*)new ExpressionRecordAccess(res, right, field);
            } else {
                res = (Expression*)new ExpressionRecordAccess(res, right, nullptr);
            }
        } else if (lex.token == OPEN_SQUARE_BRACKET) {
            std::vector<Expression*> indecies = ParseArrayIndices(table);
            res = (Expression*)new ExpressionArrayIndecies(res, indecies);
            scanner.CheckCurLexem(CLOSE_SQUARE_BRACKET, "]");
        } else if (lex.token == OPEN_BRACKET) {
            std::vector <Expression*> args;
            scanner.NextToken();
            if (scanner.GetLexem().token != CLOSE_BRACKET) {
                args.push_back(ParseExpression(table, 0));
                while (scanner.GetLexem().token == COMMA) {
                    scanner.NextToken();
                    args.push_back(ParseExpression(table, 0));
                }
            }
            if (scanner.GetLexem().token != CLOSE_BRACKET) {
                throw;
            }
            res = (Expression*)new ExpressionFuncCall(res, args);
        } else if (lex.token == CIRCUMFLEX) {
            res = (Expression*)new ExpressionDereference(res);
        }
        else {
            flag = false;
        }
    }
    return res;
}

std::vector <Expression*> Parser::ParseArrayIndices(SymbolTable* table) {
    std::vector <Expression*> exprs;
    Lexem lex;
    scanner.NextToken();
    exprs.push_back(ParseExpression(table, 0));
    lex = scanner.GetLexem();
    while (lex.token == COMMA) {
        scanner.NextToken();
        exprs.push_back(ParseExpression(table, 0));
        lex = scanner.GetLexem();
    }
    return exprs;
}

void Parser::CheckNextLexem(Lexem cur, Lexem expc) {
    if (cur.token != expc.token) {
        cur.pos.second++;
        throw ParserException("Illegal expression: expected '" + expc.val + "' " + cur.GetStrPos());
    }
}

void Parser::ParseDeclaration(SymbolTable* table) {
    Lexem lex = scanner.GetLexem();
    while (scanner.GetLexem().token != T_EOF) {
        if (scanner.GetLexem().token == CONST) {
            ParseConstantDeclaration(table);
            continue;
        }
        if (scanner.GetLexem().token == LABEL) {
            ParseLabelDeclaration(table);
            continue;
        }
        if (scanner.GetLexem().token == TYPE) {
            ParseTypeDeclaration(table);
            continue;
        }
        if (scanner.GetLexem().token == VAR) {
            ParseVarDeclaration(table);
            continue;
        }
        if (scanner.GetLexem().token == FUNCTION) {
            ParseFuncDeclaration(table, DeclarationType::FUNC);
            continue;
        }
        if (scanner.GetLexem().token == PROCEDURE) {
            ParseFuncDeclaration(table, DeclarationType::PROCEDURE);
            continue;
        }
        if (scanner.GetLexem().token == BEGIN) {
            return;
        }
        throw UnexpectedExpression("begin", scanner.GetLexem().val, scanner.GetLexem().pos);
    }
    for (auto it : declForwardCall) {
        if (((SymbolCall*)(it.sym))->block == nullptr) {
            throw ForwardDecl(it.sym->name, it.pos);
        }
    }
}

void Parser::ParseLabelDeclaration(SymbolTable* table) {
    scanner.CheckNextLexem(IDENTIFIER, "IDENTIFIER");
    while (scanner.GetLexem().token == IDENTIFIER) {
        table->CheckLocalSymbol(scanner.GetLexem().val, scanner.GetLexem().pos);
        table->Add(new SymbolLabel(scanner.GetLexem().val));
        scanner.CheckNextLexem(SEMI_COLON, ";");
        scanner.NextToken();
    }
} 

void Parser::ParseFuncDeclaration(SymbolTable* table, DeclarationType declarationType) {
    scanner.CheckNextLexem(IDENTIFIER, "IDENTIFIER");
    std::pair<int, int> pos = scanner.GetLexem().pos;
    std::string name = scanner.GetLexem().val;
    scanner.NextToken();
    SymbolTable* localTable = new SymbolTable(table);
    int argc = 0;
    if (scanner.GetLexem().token == OPEN_BRACKET) {
        scanner.NextToken();
        argc = ParseArguments(localTable);
        scanner.CheckCurLexem(CLOSE_BRACKET, ")");
        scanner.NextToken();
    }
    Symbol* newSymbol = nullptr;
    if (declarationType == DeclarationType::FUNC) {
        scanner.CheckCurLexem(COLON, ":");
        scanner.NextToken();
        Symbol* type = ParseType(table);
        localTable->Add(new SymbolVar("Result", nullptr, type, ArgTypeState::RVALUE));
        scanner.CheckCurLexem(SEMI_COLON, ";");
        scanner.NextToken();
        newSymbol = new SymbolFunction(name, localTable, nullptr, argc + 1, type);
    }
    if (declarationType == DeclarationType::PROCEDURE) {
        scanner.CheckCurLexem(SEMI_COLON, ";");
        scanner.NextToken();
        newSymbol = new SymbolProcedure(name, localTable, nullptr, argc);
    }
    auto symbols = table->GetAllSymbols(name, pos);
    SymbolCall* firstDeclSym = nullptr;
    for (auto it = symbols.begin(); it != symbols.end(); it++) {
        if (CmpArguments().Compare(newSymbol, *it)) {
            if ((*it)->declType == declarationType && ((SymbolCall*)(*it))->block == nullptr) {
                firstDeclSym = (SymbolCall*)(*it);
                localTable = firstDeclSym->symbolTable;
            } else {
                throw DuplicateIdent(name, pos);
            }
        }
    }
    if (scanner.GetLexem().token == FORWARD) {
        if (firstDeclSym != nullptr) {
            throw DuplicateIdent(name, pos);
        }
        scanner.CheckNextLexem(SEMI_COLON, ";");
        scanner.NextToken();
        declForwardCall.push_back({newSymbol, pos});
        table->Add(newSymbol);
        return;
    }

    ParseDeclaration(localTable);
    scanner.CheckCurLexem(BEGIN, "begin");
    if (firstDeclSym != nullptr) {
        firstDeclSym->block = ParseBlock(localTable, 0);
    } else {
        table->Add(newSymbol);
        ((SymbolCall*)newSymbol)->block = ParseBlock(localTable, 0);
    }
}

void Parser::ParseVarDeclaration(SymbolTable* table) {
    scanner.CheckNextLexem(IDENTIFIER, "IDENTIFIER");
    while (scanner.GetLexem().token == IDENTIFIER) {
        std::vector<std::string> names;
        names.push_back(scanner.GetLexem().val);
        std::vector<std::pair<int, int>> namesPos;
        namesPos.push_back(scanner.GetLexem().pos);
        scanner.NextToken();
        while (scanner.GetLexem().token == COMMA) {
            scanner.CheckNextLexem(IDENTIFIER, "IDENTIFIER");
            names.push_back(scanner.GetLexem().val);
            namesPos.push_back(scanner.GetLexem().pos);
            scanner.NextToken();
        }
        scanner.CheckCurLexem(COLON, ":");
        scanner.NextToken();
        Symbol* type = ParseType(table);
        ArgTypeState state = ArgTypeState::RVALUE;
        while (type->declType == DeclarationType::TYPE && ((SymbolType*)type)->dataType == DataType::BADTYPE) {
            type = ((SymbolType*)type)->type;
        }
        if (names.size() > 1 && scanner.GetLexem().token == EQUAL) {
            //throw ParserException("Illegal expression: expected ';' " + scanner.GetLexem().GetStrPos());
            throw UnexpectedExpression(";", scanner.GetLexem().val, scanner.GetLexem().pos);
        }
        Expression* exp = nullptr;
        if (scanner.GetLexem().token == EQUAL) {
            std::pair <int, int> pos = scanner.GetLexem().pos;
            exp = ParseInit(table);
            exp->typeID = ((SymbolType*)type)->dataType;
            if (testType) {
                TypeChecker(table, type, exp, scanner.GetLexem().pos);
            }
        }
        for (int i = 0; i < names.size(); i++) {
            table->CheckLocalSymbol(names[i], namesPos[i]);
            table->Add(new SymbolVar(names[i], exp, type, state));
        }
        scanner.CheckCurLexem(SEMI_COLON, ";");
        scanner.NextToken();
    }
}

void Parser::ParseTypeDeclaration(SymbolTable* table) {
    scanner.NextToken();
//    Lexem lex = scanner.GetLexem();
//    Symbol* sym = nullptr;
//    try {
//        sym = table->GetSymbol(scanner.GetLexem().val, scanner.GetLexem().pos);
//    } catch (ParserException error) {
//        ;
//    }
//    if (sym != nullptr) {
//        throw ParserException("Duplicate IDENTIFIER '" + scanner.GetLexem().val + "' " + scanner.GetLexem().GetStrPos());
//    }
    scanner.CheckCurLexem(IDENTIFIER, "IDENTIFIER");
    while (scanner.GetLexem().token == IDENTIFIER) {
        std::string name = scanner.GetLexem().val;
        std::pair<int, int> pos = scanner.GetLexem().pos;
        scanner.CheckNextLexem(EQUAL, "=");
        scanner.NextToken();
        table->CheckLocalSymbol(name, pos);
        table->Add(new SymbolType(name, ParseType(table)));
        scanner.CheckCurLexem(SEMI_COLON, ";");
        scanner.NextToken();
    }
}

void Parser::ParseConstantDeclaration(SymbolTable* table) {
    scanner.CheckNextLexem(IDENTIFIER, "IDENTIFIER");
    while (scanner.GetLexem().token == IDENTIFIER) {
        std::string name = scanner.GetLexem().val;
        std::pair <int, int> namePos = scanner.GetLexem().pos;
        scanner.NextToken();
        Symbol* type = nullptr;
        if (scanner.GetLexem().token == COLON) {
            scanner.NextToken();
            type = ParseType(table);
            while (((SymbolType*)type)->dataType == DataType::BADTYPE) {
                type = type->GetType();
            }
        }
        scanner.CheckCurLexem(EQUAL, "=");
        std::pair<int, int> pos = scanner.GetLexem().pos;
        Expression* exp = ParseInit(table);
        ArgTypeState state = ArgTypeState::RVALUE;
        if (type != nullptr) {
            TypeChecker(table, type, exp, scanner.GetLexem().pos);
        } else {
            DataType typeID = TypeChecker(table, scanner.GetLexem().pos).GetTypeID(exp);
            type = new SymbolType(dataTypeString[(int)typeID], typeID);
            if (typeID == DataType::ARRAY || typeID == DataType::POINTER) {
                state = ArgTypeState::VAR;
            }
        }
        table->CheckLocalSymbol(name, namePos);
        table->Add(new SymbolConst(name, exp, type, ArgTypeState::RVALUE));
        scanner.CheckCurLexem(SEMI_COLON, ";");
        scanner.NextToken();
    }
}

Symbol* Parser::ParseType(SymbolTable* table) {
    if (scanner.GetLexem().token == ARRAY) {
        return ParseArrayDecl(table);
    }
    else if (scanner.GetLexem().token == STRING) {
        return ParseString(table);
    }
    else if (scanner.GetLexem().token == RECORD) {
        return ParseRecord(table);
    } else {
        bool flag = false;
        if (scanner.GetLexem().token == CIRCUMFLEX) {
            flag = true;
            scanner.NextToken();
        }
        std::pair<int, int> pos = scanner.GetLexem().pos;
        if (!table->Find(scanner.GetLexem().val)) {
            throw TypeNotFound(scanner.GetLexem().val, pos);
        }
        Symbol* sym = table->GetSymbol(scanner.GetLexem().val, scanner.GetLexem().pos);
        while (((SymbolType*)sym)->dataType == DataType::BADTYPE) {
            Symbol* _sym = table->GetSymbol(sym->name, pos);
            sym = _sym->GetType();
        }
        scanner.NextToken();
        if (flag) {
            return new SymbolPointer("pointer", sym);
        }
        return sym;
    }
}

int Parser::ParseArguments(SymbolTable* table) {
    int res = 0;
    while (scanner.GetLexem().token == IDENTIFIER || scanner.GetLexem().token == CONST ||
            scanner.GetLexem().token == VAR) {
        ArgTypeState state = ArgTypeState::RVALUE;
        if (scanner.GetLexem().token == CONST) {
            state = ArgTypeState::CONST;
            scanner.NextToken();
        }
        if (scanner.GetLexem().token == VAR) {
            state = ArgTypeState::VAR;
            scanner.NextToken();
        }
        ++res;
        std::pair<int, int> pos = scanner.GetLexem().pos;
        std::string name = scanner.GetLexem().val;
        scanner.CheckNextLexem(COLON, ":");
        scanner.NextToken();
        Symbol* type = ParseType(table);
        table->CheckLocalSymbol(name, pos);
        table->Add(new SymbolVar(name, nullptr, type, state));
        if (scanner.GetLexem().token != END && scanner.GetLexem().token != CLOSE_BRACKET) {
            scanner.CheckCurLexem(SEMI_COLON, ";");
            scanner.NextToken();
        }
    }
    return res;
}

Symbol* Parser::ParseRecord(SymbolTable* table) {
    scanner.NextToken();
    SymbolTable* tempTable = new SymbolTable(table);
    int argc = ParseArguments(tempTable);
    scanner.CheckCurLexem(END, "END");
    scanner.NextToken();
    return new SymbolRecord(tempTable, "", argc);
}

Symbol* Parser::ParseArrayDecl(SymbolTable* table) {
    scanner.NextToken();
    if (scanner.GetLexem().token == OPEN_SQUARE_BRACKET) {
        scanner.NextToken();
        Expression* leftExpr = ParseExpression(table, 0);
        CheckConstant(symTable, leftExpr);
        int A = CalculateExpression<int>(table, "INTEGER", scanner.GetLexem().pos).Calculate(leftExpr);
        scanner.CheckCurLexem(DOUBLE_POINT, "..");
        scanner.NextToken();
        Expression* rightExpr = ParseExpression(table, 0);
        CheckConstant(symTable, rightExpr);
        int B = CalculateExpression<int>(table, "INTEGER", scanner.GetLexem().pos).Calculate(rightExpr);
        SymbolArray* symbol = new SymbolArray(nullptr, A, B);
        Symbol** symbolTypeInitialize = &symbol->type;
        while (scanner.GetLexem().token == COMMA) {
            scanner.NextToken();
            leftExpr = ParseExpression(table, 0);
            CheckConstant(symTable, leftExpr);
            A = CalculateExpression<int>(table, "INTEGER", scanner.GetLexem().pos).Calculate(leftExpr);
            scanner.CheckCurLexem(DOUBLE_POINT, "..");
            scanner.NextToken();
            Expression* rightExpr = ParseExpression(table, 0);
            CheckConstant(symTable, rightExpr);
            B = CalculateExpression<int>(table, "INTEGER", scanner.GetLexem().pos).Calculate(rightExpr);
            *symbolTypeInitialize = new SymbolArray(nullptr, A, B);
            symbolTypeInitialize = &((SymbolArray*)*symbolTypeInitialize)->type;
        }
        scanner.CheckCurLexem(CLOSE_SQUARE_BRACKET, "]");
        scanner.CheckNextLexem(OF, "of");
        std::pair <int, int> pos = scanner.GetLexem().pos;
        scanner.NextToken();
        *symbolTypeInitialize = ParseType(symTable);
        while (((SymbolType*)*symbolTypeInitialize)->dataType == DataType::BADTYPE) {
            Symbol* sym = symTable->GetSymbol((*symbolTypeInitialize)->name, pos);
            *symbolTypeInitialize = sym->GetType();
        }
        return symbol;
    }
    scanner.CheckCurLexem(OF, "of");
    scanner.NextToken();
    return new SymbolDynArray(ParseType(table));
}

Symbol* Parser::ParseString(SymbolTable* table) {
    scanner.NextToken();
    int length = -1;
    if (scanner.GetLexem().token == OPEN_SQUARE_BRACKET) {
        scanner.NextToken();
        std::pair<int, int> pos = scanner.GetLexem().pos;
        auto expLength = ParseExpression(table, 0);
        CheckConstant(table, expLength);
        length = CalculateExpression<int>(table, "INTEGER", pos).Calculate(expLength);
        scanner.CheckCurLexem(CLOSE_SQUARE_BRACKET, "]");
        scanner.NextToken();
    }
    auto temp = new SymbolString(length);
    return temp;
}

Expression* Parser::ParseInit(SymbolTable* table) {
    scanner.NextToken();
    int count = 0;
    if (scanner.GetLexem().token == OPEN_BRACKET) {
        Expression* exp = ParseInitializeList(table);
        scanner.CheckCurLexem(CLOSE_BRACKET, ")");
        scanner.NextToken();
        return exp;
    }
    Expression* exp = nullptr;
    try {
        exp = ParseExpression(table, 0);
    } catch (ParserException error) {
        throw ParserException("Illegal expression: expected 'IDENTIFIER' " + scanner.GetLexem().GetStrPos());
    }

    CheckConstant(table, exp);
    return exp;
}

Expression* Parser::ParseInitializeList(SymbolTable* table) {
    ExpressionInitializeList* res = new ExpressionInitializeList();
    do {
        scanner.NextToken();
        if (scanner.GetLexem().token == OPEN_BRACKET) {
            res->initList.push_back(ParseInitializeList(table));
            scanner.CheckCurLexem(CLOSE_BRACKET, ")");
            scanner.NextToken();
            if (scanner.GetLexem().token != COMMA) {
                return res;
            }
            continue;
        }
        Expression* exp = ParseExpression(table, 0);
        CheckConstant(table, exp);
        res->initList.push_back(exp);
    } while (scanner.GetLexem().token == COMMA);
    return res;
}

void Parser::CheckConstant(SymbolTable* table, Expression* expr) {
    ExpressionArgumentList* list = new ExpressionArgumentList();
    expr->GetIdentificitationList(list);
    if (list->flag == false) {
        throw ExpectedConstExpression(scanner.GetLexem().pos);
    }
    for (auto symbol: list->arguments) {
        if (table->Find(symbol) != false && table->GetSymbol(symbol, scanner.GetLexem().pos)->declType != DeclarationType::CONST) {
//            throw ParserException("not const expression " + scanner.GetLexem().GetStrPos());
            throw ExpectedConstExpression(scanner.GetLexem().pos);
        }
    }
}

Block* Parser::ParseBlock(SymbolTable* table, int state) {
    if (scanner.GetLexem().token == BEGIN) {
        return ParseCompoundBlock(table, state);
    } else if (scanner.GetLexem().token == GOTO) {
        return ParseGoToBlock(table, state);
    } else if (scanner.GetLexem().token == TRY) {
        return ParseTryBlock(table, state);
    } else if (scanner.GetLexem().token == FOR) {
        return ParseForBlock(table, state);
    } else if (scanner.GetLexem().token == IDENTIFIER) {
        return ParseBlockIdent(table, state);
    } else if (scanner.GetLexem().token == WHILE) {
        return ParseWhileBlock(table, state);
    } else if (scanner.GetLexem().token == SEMI_COLON) {
        scanner.NextToken();
        return nullptr;
    } else if (scanner.GetLexem().token == CASE) {
        return ParseCaseBlock(table, state);
    } else if (scanner.GetLexem().token == EXCEPT ||
               scanner.GetLexem().token == FINALLY ||
               scanner.GetLexem().token == END ||
               scanner.GetLexem().token == ELSE) {
        return nullptr;
    }
    else if (scanner.GetLexem().token == CONTINUE) {
        if (state & 2) {
//            scanner.CheckNextLexem(SEMI_COLON, ";");
            scanner.NextToken();
            CheckSemiColon();
            return new BlockContinue();
        }
    } else if (scanner.GetLexem().token == RAISE) {
        return ParseRaiseBlock(table, state);
    }
    else if (scanner.GetLexem().token == IF) {
        return ParseIfBlock(table, state);
    } else if (scanner.GetLexem().token == REPEAT) {
        return ParseRepeatBlock(table, state);
    } else if (scanner.GetLexem().token == BREAK) {
        if (state & 2) {
            scanner.NextToken();
            CheckSemiColon();
            return new BlockBreak();
        }
    }
    else {
//        throw ParserException("Illegal Expression in pos " + scanner.GetLexem().GetStrPos());
        throw UnexpectedExpression("begin", scanner.GetLexem().val, scanner.GetLexem().pos);
    }
}

Block* Parser::ParseWhileBlock(SymbolTable* table, int state) {
    scanner.NextToken();
    std::pair<int, int> pos = scanner.GetLexem().pos;
    Expression* cond = ParseExpression(table, 0);
    if (testType) {
        TypeChecker(table, DataType::BOOLEAN, cond, scanner.GetLexem().pos);
    }
    scanner.CheckCurLexem(DO, "do");
    scanner.NextToken();
    return new BlockWhile(cond, ParseBlock(table, state | 2));
}

std::set<TokenType> endBlockToken = {END, ELSE, UNTIL, POINT, FINALLY, EXCEPT};


void Parser::CheckSemiColon() {
    if (endBlockToken.find(scanner.GetLexem().token) == endBlockToken.cend()) {
        scanner.CheckCurLexem(SEMI_COLON, ";");
        scanner.NextToken();
    }
}

Block* Parser::ParseCompoundBlock(SymbolTable* table, int state) {
    scanner.CheckCurLexem(BEGIN, "begin");
    scanner.NextToken();
    BlockCompound* blockCompound = new BlockCompound();
    blockCompound->listBlock = ParseBlockList(table, state);
    scanner.CheckCurLexem(END, "end");
    scanner.NextToken();
    if (scanner.GetLexem().token == ELSE) {
        return blockCompound;
    }
    // scanner.CheckCurLexem(SEMI_COLON, ";");
    // scanner.NextToken();
    CheckSemiColon();
    return blockCompound;
}

std::vector<Block*> Parser::ParseBlockList(SymbolTable* table, int state) {
    std::vector<Block*> ans;
    while (endBlockToken.find(scanner.GetLexem().token) == endBlockToken.cend()) {
        Block* block = ParseBlock(table, state);
        if (block != nullptr) {
            ans.push_back(block);
        }
    }
    return ans;
}

Block* Parser::ParseForBlock(SymbolTable* table, int state) {
    scanner.NextToken();
    std::pair<int, int> pos = scanner.GetLexem().pos;
    Expression* exp1 = ParseExpression(table, 0);
    if (testType) {
        TypeChecker(table, exp1, scanner.GetLexem().pos);
    }
    bool toFlag;
    if (scanner.GetLexem().token == DOWNTO) {
        scanner.CheckCurLexem(DOWNTO, "downto");
        toFlag = false;
    } else {
        scanner.CheckCurLexem(TO, "to");
        toFlag = true;
    }
    pos = scanner.GetLexem().pos;
    scanner.NextToken();
    Expression* exp2 = ParseExpression(table, 0);
    if (testType) {
        TypeChecker(table, DataType::INTEGER, exp2, scanner.GetLexem().pos);
    }
    scanner.CheckCurLexem(DO, "do");
    scanner.NextToken();
    Block* block = ParseBlock(table, state | 2);
    return new BlockFor(exp1, exp2, toFlag, block);
}

std::set<DataType> ordinalTypes = {DataType::STRING, DataType::CHAR, DataType::INTEGER, DataType::BOOLEAN};

Block* Parser::ParseCaseBlock(SymbolTable* table, int state) {
    scanner.NextToken();
    auto pos = scanner.GetLexem().pos;
    auto exp = ParseExpression(table, state);
    auto dataType = exp->typeID;
    if (ordinalTypes.find(dataType) == ordinalTypes.end()) {
        throw "ordinalTypeException";
    }
    auto blockCase = new BlockCase(exp);
    scanner.CheckCurLexem(OF, "of");
    scanner.NextToken();
    if (scanner.GetLexem().token == ELSE || scanner.GetLexem().token == END) {
        throw IllegalExpression(scanner.GetLexem().pos);
    }
    while (scanner.GetLexem().token != ELSE && scanner.GetLexem().token != END) {
        auto pos = scanner.GetLexem().pos;
        Expression* exp1 = ParseExpression(table, state);
        TypeChecker(table, exp, exp1, pos);
        CheckConstant(table, exp1);
        Expression* exp2 = nullptr;
        if (scanner.GetLexem().token == DOUBLE_POINT) {
            scanner.NextToken();
            pos = scanner.GetLexem().pos;
            exp2 = ParseExpression(table, state);
            TypeChecker(table, exp, exp2, pos);
        }
        scanner.CheckCurLexem(COLON, ":");
        scanner.NextToken();
        auto block = ParseBlock(table, state);
        blockCase->Add(CaseNode(exp1, exp2, block));
    }
    if (scanner.GetLexem().token == ELSE) {
        scanner.NextToken();
        blockCase->blockElse = ParseBlock(table, state);
    }
    scanner.CheckCurLexem(END, "end");
    scanner.NextToken();
    CheckSemiColon();
    return blockCase;
}



Block* Parser::ParseRaiseBlock(SymbolTable* table, int state) {
    scanner.NextToken();
    std::pair <int, int> pos = scanner.GetLexem().pos;
    auto exp = ParseExpression(table, state);
    TypeChecker(table, DataType::STRING, exp, pos);
    return new BlockRaise(exp);
}

Block* Parser::ParseBlockIdent(SymbolTable* table, int state) {
    std::pair<int, int> pos = scanner.GetLexem().pos;
    Symbol* symbol = table->GetSymbol(scanner.GetLexem().val, pos);
    if (symbol->declType == DeclarationType::LABEL) {
        scanner.NextToken();
        scanner.CheckCurLexem(COLON, ":");
        scanner.NextToken();
        return new BlockGoToLabel(symbol);
    }
    Expression* exp = ParseExpression(table, 0);
    if (exp->expressionType == ExpressionType::ASSIGN) {
        if (testType) {
            TypeChecker(table, exp, pos);
        }
        CheckSemiColon();
        return new BlockAssign(exp);
    }
    if (exp->expressionType == ExpressionType::FUNCCALL) {
        CheckSemiColon();
        return new BlockFuncCall(exp);
    }
//    throw ParserException("Illegal expression in pos " + scanner.GetLexem().GetStrPos());
    throw IllegalExpression(scanner.GetLexem().pos);
}

Block* Parser::ParseBlockStart() {
    this->ParseDeclaration(this->symTable);
    Block* block = new BlockCompound();
    scanner.CheckCurLexem(BEGIN, "begin");
    scanner.NextToken();
    ((BlockCompound*)block)->listBlock = this->ParseBlockList(this->symTable, 0);
    scanner.CheckNextLexem(POINT, ".");
    return block;
}

Block* Parser::ParseIfBlock(SymbolTable* table, int state) {
    scanner.NextToken();
    std::pair <int, int> pos = scanner.GetLexem().pos;
    Expression* exp = ParseExpression(table, 0);
    if (testType) {
        TypeChecker(table, DataType::BOOLEAN, exp, scanner.GetLexem().pos);
    }
    scanner.CheckCurLexem(THEN, "then");
    scanner.NextToken();
    Block* blockThen = ParseBlock(symTable, state);
    Block* blockElse = nullptr;
    if (scanner.GetLexem().token == ELSE) {
        scanner.NextToken();
        blockElse = ParseBlock(table, state);
    }
    return new BlockIf(exp, blockThen, blockElse);
}

Block* Parser::ParseRepeatBlock(SymbolTable* table, int state) {
    scanner.NextToken();
    std::vector<Block*> blocks = ParseBlockList(table, state | 2);
    scanner.CheckCurLexem(UNTIL, "until");
    scanner.NextToken();
    Expression* exp = ParseExpression(table, 0);
    return new BlockRepeat(exp, blocks);
}

Block* Parser::ParseGoToBlock(SymbolTable* table, int state) {
    scanner.CheckNextLexem(IDENTIFIER, "IDENTIFIER");
    auto symbol = table->GetSymbol(scanner.GetLexem().val, scanner.GetLexem().pos);
    scanner.CheckNextLexem(SEMI_COLON, ";");
    return new BlockGoTo(symbol);
}

Block* Parser::ParseTryBlock(SymbolTable* table, int state) {
    scanner.NextToken();
    auto block1 = ParseBlockList(table, state);
    std::vector <Block*> block2;
    if (scanner.GetLexem().token == EXCEPT) {
        scanner.NextToken();
        block2 = ParseBlockList(table, state);
        scanner.CheckCurLexem(END, "end");
        scanner.NextToken();
        return new BlockTryExcept(block1, block2);
    }
    if (scanner.GetLexem().token == FINALLY) {
        scanner.NextToken();
        block2 = ParseBlockList(table, state);
        scanner.CheckCurLexem(END, "end");
        scanner.NextToken();
        return new BlockTryFinally(block1, block2);
    }
    throw UnexpectedExpression("EXCEPT", scanner.GetLexem().val, scanner.GetLexem().pos);
}