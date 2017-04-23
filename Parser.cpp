#include "Parser.h"
//#include "CalculateExpression.h"

void Parser::Print() {
    expression = ParseExpression(0);
    if (scanner.GetLexem().token != TokenType::T_EOF)
        throw ParserException("Illegal expression in pos " + scanner.GetLexem().GetStrPos());
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
    priorities[LESS_OR_EQUAL_THAN];
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
    priorities[SHIFT_RIGHT] = 2;
    priorities[SHIFT_LEFT] = 2;

    unarPriorities[NOT] = 3;
    unarPriorities[SUB] = 3;
    unarPriorities[ADD] = 3;



//    expression = ParseExpression(0);
//    if (scanner.GetLexem().token != TokenType::T_EOF)
//        throw ParserException("Illegal expression in pos " + scanner.GetLexem().GetStrPos());
}

Expression* Parser::ParseExpression(int priority) {
    if (priority == 3) {
        return ParseFactor();
    }
    Expression* res = ParseExpression(priority + 1);
    Lexem lex = scanner.GetLexem();
    while (PriorityCheck(priority, lex.token)) {
        if (scanner.GetLexem().token == ASSIGNMENT) {
            scanner.NextToken();
            Expression* right = ParseExpression(priority + 1);
            res = (Expression*)new ExpressionAssign(right, res);
            lex = scanner.GetLexem();
            return res;
        }
        scanner.NextToken();
        Expression* right = ParseExpression(priority + 1);
        res = (Expression*)new ExpressionBinOp(lex, res, right);
        lex = scanner.GetLexem();
    }
    return res;
}

Expression* Parser::ParseFactor() {
    auto lex = scanner.GetLexem();
    if (PriorityCheck(3, lex.token)) {
        scanner.NextToken();
        auto curExp = ParseExpression(0);
        return (Expression*)new ExpressionUnOp(lex, curExp);
    }
    if (lex.token == OPEN_BRACKET) {
        scanner.NextToken();
        auto curExp = ParseExpression(0);
        CheckNextLexem(scanner.GetLexem(), Lexem(")", CLOSE_BRACKET));
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
    if (lex.token == IDENTIFICATOR) {
        return ParseTerm(true);
    }
    throw ParserException("Illegal expression in pos " + lex.GetStrPos());
}

bool Parser::PriorityCheck(int priority, TokenType token) {
    return (priorities[token] == priority || unarPriorities[token] == priority);
}

Expression *Parser::ParseTerm(bool flag) {
    Lexem lex = scanner.GetLexem();
    Expression* res = (Expression*)new ExpressionIdent(lex);
    while (flag) {
        scanner.NextToken();
        lex = scanner.GetLexem();
        if (lex.token == POINT) {
            scanner.NextToken();
            lex = scanner.GetLexem();
            CheckNextLexem(lex, Lexem("IDENTIFICATOR", IDENTIFICATOR));
            Expression* right = ParseTerm(false);
            res = (Expression*)new ExpressionRecordAccess(res, right);
        } else if (lex.token == OPEN_SQUARE_BRACKET) {
            std::vector<Expression*> indecies = ParseArrayIndices();
            res = (Expression*)new ExpressionArrayIndecies(res, indecies);
            lex = scanner.GetLexem();
            CheckNextLexem(lex, Lexem("]", CLOSE_SQUARE_BRACKET));
        } else if (lex.token == OPEN_BRACKET) {
            std::vector <Expression*> args;
            scanner.NextToken();
            if (scanner.GetLexem().token != CLOSE_BRACKET) {
                args.push_back(ParseExpression(0));
                while (scanner.GetLexem().token == COMMA) {
                    scanner.NextToken();
                    args.push_back(ParseExpression(0));
                }
            }
            if (scanner.GetLexem().token != CLOSE_BRACKET) {
                throw;
            }
            scanner.NextToken();
            res = (Expression*)new ExpressionFuncCall(res, args);
        } else {
            flag = false;
        }
    }
    return res;
}

std::vector <Expression*> Parser::ParseArrayIndices() {
    std::vector <Expression*> exprs;
    Lexem lex;
    scanner.NextToken();
    exprs.push_back(ParseExpression(0));
    lex = scanner.GetLexem();
    while (lex.token == COMMA) {
        scanner.NextToken();
        exprs.push_back(ParseExpression(0));
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

void Parser::ParseDeclaration(SymbolTable* symTable) {
    Lexem lex = scanner.GetLexem();
    while (scanner.GetLexem().token != T_EOF) {
        if (scanner.GetLexem().token == CONST) {
            ParseConstantDeclaration(symTable);
            continue;
        }
        if (scanner.GetLexem().token == TYPE) {
            ParseTypeDeclaration(symTable);
            continue;
        }
        if (scanner.GetLexem().token == VAR) {
            ParseVarDeclaration(symTable);
            continue;
        }
        if (scanner.GetLexem().token == FUNCTION) {
            ParseFuncDeclaration(symTable, DeclarationType::FUNC);
            continue;
        }
        if (scanner.GetLexem().token == PROCEDURE) {
            ParseFuncDeclaration(symTable, DeclarationType::PROCEDURE);
            continue;
        }
        if (scanner.GetLexem().token == BEGIN) {
            return;
        }

    }
}

void Parser::ParseFuncDeclaration(SymbolTable* symbolTable, DeclarationType declarationType) {
    scanner.NextToken();
    CheckNextLexem(scanner.GetLexem(), Lexem("IDENTIFICATOR", IDENTIFICATOR));
    std::pair<int, int> pos = scanner.GetLexem().pos;
    std::string name = scanner.GetLexem().val;
    scanner.NextToken();
    SymbolTable* localTable = new SymbolTable(symbolTable);
    int argc = 0;
    if (scanner.GetLexem().token == OPEN_BRACKET) {
        scanner.NextToken();
        int argc = ParseArguments(symbolTable);
        CheckNextLexem(scanner.GetLexem(), Lexem(")", CLOSE_BRACKET));
        scanner.NextToken();
    }
    Symbol* newSymbol = nullptr;
    if (declarationType == DeclarationType::FUNC) {
        CheckNextLexem(scanner.GetLexem(), Lexem(":", COLON));
        scanner.NextToken();
        Symbol* type = ParseType(symbolTable);
        localTable->Add(new SymbolVar("Result", nullptr, type, ArgumentType::RVALUE));
        CheckNextLexem(scanner.GetLexem(), Lexem(";", SEMI_COLON));
        scanner.NextToken();
        newSymbol = new SymbolFunction(name, localTable, nullptr, argc + 1, type);
    }
    if (declarationType == DeclarationType::PROCEDURE) {
        CheckNextLexem(scanner.GetLexem(), Lexem(";", SEMI_COLON));
        scanner.NextToken();
        newSymbol = new SymbolProcedure(name, localTable, nullptr, argc);
    }
    std::vector <Symbol*> symbols = symbolTable->GetAllSymbols(name, pos);
    SymbolCall* firstDecl = nullptr;
    ParseDeclaration(localTable);
    CheckNextLexem(scanner.GetLexem(), Lexem("begin", BEGIN));
    symbolTable->Add(newSymbol);
    ((SymbolCall*)newSymbol)->block = ParseBlock(localTable, 0);
}

void Parser::ParseVarDeclaration(SymbolTable* symTable) {
    scanner.NextToken();
    Lexem lex = scanner.GetLexem();
    CheckNextLexem(lex, Lexem("IDENTIFICATOR", IDENTIFICATOR));
    while (scanner.GetLexem().token == IDENTIFICATOR) {
        std::vector<std::string> names;
        names.push_back(scanner.GetLexem().val);
        std::vector<std::pair<int, int>> namesPos;
        namesPos.push_back(scanner.GetLexem().pos);
        scanner.NextToken();
        while (scanner.GetLexem().token == COMMA) {
            scanner.NextToken();
            Lexem lex = scanner.GetLexem();
            CheckNextLexem(lex, Lexem("IDENTIFICATOR", IDENTIFICATOR));
            names.push_back(scanner.GetLexem().val);
            namesPos.push_back(scanner.GetLexem().pos);
            scanner.NextToken();
        }
        Lexem lex = scanner.GetLexem();
        CheckNextLexem(lex, Lexem(":", COLON));
        scanner.NextToken();
        Symbol* type = ParseType(symTable);
        ArgumentType argType = ArgumentType::RVALUE;
        if (strcasecmp(type->name.c_str(), "pointer") == 0 || type->name.length() == 0) {
            argType = ArgumentType::RVALUE;
        }
        while (type->declType == DeclarationType::TYPE && ((SymbolType*)type)->dataType == DataType::BADTYPE) {
            type = ((SymbolType*)type)->type;
        }
        if (names.size() > 1 && scanner.GetLexem().token == EQUAL) {
            throw;
        }
        Expression* exp = nullptr;
        if (scanner.GetLexem().token == EQUAL) {
            std::pair <int, int> pos = scanner.GetLexem().pos;
            exp = ParseInit(symTable);
        }
        for (int i = 0; i < names.size(); i++) {
            symTable->CheckLocalSymbol(names[i], namesPos[i]);
            symTable->Add(new SymbolVar(names[i], exp, type, argType));
        }
        lex = scanner.GetLexem();
        CheckNextLexem(lex, Lexem(";", SEMI_COLON));
        scanner.NextToken();
    }
}

void Parser::ParseTypeDeclaration(SymbolTable* symTable) {
    scanner.NextToken();
    Lexem lex = scanner.GetLexem();
    CheckNextLexem(lex, Lexem("IDENTIFICATOR", IDENTIFICATOR));
    while (scanner.GetLexem().token == IDENTIFICATOR) {
        std::string name = scanner.GetLexem().val;
        std::pair<int, int> pos = scanner.GetLexem().pos;
        scanner.NextToken();
        lex = scanner.GetLexem();
        CheckNextLexem(lex, Lexem("=", EQUAL));
        scanner.NextToken();
        symTable->CheckLocalSymbol(name, pos);
        symTable->Add(new SymbolType(name, ParseType(symTable)));
//        scanner.NextToken();
        lex = scanner.GetLexem();
        CheckNextLexem(lex, Lexem(";", SEMI_COLON));
        scanner.NextToken();
    }
}

void Parser::ParseConstantDeclaration(SymbolTable* symTable) {
    scanner.NextToken();
    Lexem lex = scanner.GetLexem();
    CheckNextLexem(lex, Lexem("IDENTIFICATOR", IDENTIFICATOR));
    while (scanner.GetLexem().token == IDENTIFICATOR) {
        std::string name = scanner.GetLexem().val;
        std::pair <int, int> namePos = scanner.GetLexem().pos;
        scanner.NextToken();
        Symbol* type = nullptr;
        if (scanner.GetLexem().token == COLON) {
            scanner.NextToken();
            type = ParseType(symTable);
            while (((SymbolType*)type)->dataType == DataType::BADTYPE) {
                type = type->GetType();
            }
        }
        CheckNextLexem(scanner.GetLexem(), Lexem("=", EQUAL));
        std::pair<int, int> pos = scanner.GetLexem().pos;
        Expression* exp = ParseInit(symTable);
        ArgumentType state = ArgumentType::RVALUE;
        if (type != nullptr) {
            //asd
        }
        //asd
        symTable->CheckLocalSymbol(name, namePos);
        symTable->Add(new SymbolConst(name, exp, type, ArgumentType::RVALUE));
        CheckNextLexem(scanner.GetLexem(), Lexem(";", SEMI_COLON));
        scanner.NextToken();
    }
}

Symbol* Parser::ParseType(SymbolTable* symTable) {
    if (scanner.GetLexem().token == ARRAY) {
        return ParseArrayDecl(symTable);
    }
    else if (scanner.GetLexem().token == RECORD) {
        return ParseRecord(symTable);
    } else {
        bool flag = false;
        if (scanner.GetLexem().token == CIRCUMFLEX) {
            flag = true;
            scanner.NextToken();
        }
        std::pair<int, int> pos = scanner.GetLexem().pos;
        Symbol* sym = symTable->GetSymbol(scanner.GetLexem().val, scanner.GetLexem().pos);
        while (((SymbolType*)sym)->dataType == DataType::BADTYPE) {
            Symbol* _sym = symTable->GetSymbol(sym->name, pos);
            sym = _sym->GetType();
        }
        scanner.NextToken();
        if (flag) {
            return new SymPointer("pointer", sym);
        }
        return sym;
    }
}

int Parser::ParseArguments(SymbolTable* symTable) {
    int res = 0;
    while (scanner.GetLexem().token == IDENTIFICATOR || scanner.GetLexem().token == CONST ||
            scanner.GetLexem().token == VAR) {
        ArgumentType argType = ArgumentType::RVALUE;
        if (scanner.GetLexem().token == CONST) {
            argType = ArgumentType::CONSTANT;
            scanner.NextToken();
        }
        if (scanner.GetLexem().token == VAR) {
            argType = ArgumentType::VARIABLE;
            scanner.NextToken();
        }
        ++res;
        std::pair<int, int> pos = scanner.GetLexem().pos;
        std::string name = scanner.GetLexem().val;
        scanner.NextToken();
        CheckNextLexem(scanner.GetLexem(), Lexem(":", COLON));
        scanner.NextToken();
        Symbol* type = ParseType(symTable);
        symTable->CheckLocalSymbol(name, pos);
        symTable->Add(new SymbolVar(name, nullptr, type, argType));
        if (scanner.GetLexem().token != END && scanner.GetLexem().token != CLOSE_BRACKET) {
            CheckNextLexem(scanner.GetLexem(), Lexem(";", SEMI_COLON));
            scanner.NextToken();
        }
    }
    return res;
}

Symbol* Parser::ParseRecord(SymbolTable* symbolTable) {
    scanner.NextToken();
    SymbolTable* tempTable = new SymbolTable(symbolTable);
    int argc = ParseArguments(tempTable);
    CheckNextLexem(scanner.GetLexem(), Lexem("END", END));
    scanner.NextToken();
    return new SymbolRecord(tempTable, "", argc);
}

Symbol* Parser::ParseArrayDecl(SymbolTable* symTable) {
    scanner.NextToken();
    if (scanner.GetLexem().token == OPEN_SQUARE_BRACKET) {
        scanner.NextToken();
        Expression* leftExpr = ParseExpression(0);
        leftExpr->expressionType = ExpressionType::VAR;
        CheckConstant(symTable, leftExpr);
//        scanner.NextToken();
        Lexem lex = scanner.GetLexem();
        CheckNextLexem(lex, Lexem("..", DOUBLE_POINT));
        scanner.NextToken();
        Expression* rightExpr = ParseExpression(0);
        rightExpr->expressionType = ExpressionType::VAR;
        CheckConstant(symTable, rightExpr);
        SymbolArray* symbol = new SymbolArray(nullptr, leftExpr, rightExpr);
        Symbol** symbolTypeInitialize = &symbol->type;
        while (scanner.GetLexem().token == COMMA) {
            scanner.NextToken();
            leftExpr = ParseExpression(0);
            leftExpr->expressionType = ExpressionType::VAR;
            CheckConstant(symTable, leftExpr);
            Lexem lex = scanner.GetLexem();
            CheckNextLexem(lex, Lexem("..", DOUBLE_POINT));
            scanner.NextToken();
            Expression* rightExpr = ParseExpression(0);
            rightExpr->expressionType = ExpressionType::VAR;
            CheckConstant(symTable, rightExpr);
            *symbolTypeInitialize = new SymbolArray(nullptr, leftExpr, rightExpr);
            symbolTypeInitialize = &((SymbolArray*)*symbolTypeInitialize)->type;
        }
//        scanner.NextToken();
        lex = scanner.GetLexem();
        CheckNextLexem(lex, Lexem("]", CLOSE_SQUARE_BRACKET));
        scanner.NextToken();
        lex = scanner.GetLexem();
        CheckNextLexem(lex, Lexem("of", OF));
        std::pair <int, int> pos = scanner.GetLexem().pos;
        scanner.NextToken();
        *symbolTypeInitialize = ParseType(symTable);
        while (((SymbolType*)*symbolTypeInitialize)->dataType == DataType::BADTYPE) {
            Symbol* sym = symTable->GetSymbol((*symbolTypeInitialize)->name, pos);
            *symbolTypeInitialize = sym->GetType();
        }
        return symbol;
    }
    scanner.NextToken();
    Lexem lex = scanner.GetLexem();
    CheckNextLexem(lex, Lexem("of", OF));
    return nullptr;
}

Expression* Parser::ParseInit(SymbolTable* symbolTable) {
    scanner.NextToken();
    int count = 0;
    if (scanner.GetLexem().token == OPEN_BRACKET) {
        Expression* exp = ParseInitializeList(symbolTable);
        Lexem lex = scanner.GetLexem();
        CheckNextLexem(lex, Lexem(")", CLOSE_BRACKET));
        scanner.NextToken();
        return exp;
    }
    Expression* exp = ParseExpression(0);
    CheckConstant(symbolTable, exp);
    return exp;
}

Expression* Parser::ParseInitializeList(SymbolTable* symbolTable) {
    ExpressionInitializeList* res = new ExpressionInitializeList();
    do {
        scanner.NextToken();
        if (scanner.GetLexem().token == OPEN_BRACKET) {
            res->initList.push_back(ParseInitializeList(symbolTable));
//            scanner.NextToken();
            CheckNextLexem(scanner.GetLexem(), Lexem(")", CLOSE_BRACKET));
            scanner.NextToken();
            if (scanner.GetLexem().token != COMMA) {
                return res;
            }
            continue;
        }
        Expression* exp = ParseExpression(0);
        CheckConstant(symbolTable, exp);
        res->initList.push_back(exp);
    } while (scanner.GetLexem().token == COMMA);
    return res;
}

void Parser::CheckConstant(SymbolTable* symbolTable, Expression* expr) {
    ExpressionArgumentList* list = new ExpressionArgumentList();
    expr->GetIdentificitationList(list);
    if (list->flag == false) {
        throw;
    }
    for (auto symbol: list->arguments) {
//        auto temp = symbolTable->GetSymbol(symbol, scanner.GetLexem().pos)->declType;
        if (symbolTable->Find(symbol) != false && symbolTable->GetSymbol(symbol, scanner.GetLexem().pos)->declType != DeclarationType::CONST) {
            throw;
        }
    }
}

Block* Parser::ParseBlock(SymbolTable* symbolTable, int state) {
    if (scanner.GetLexem().token == BEGIN) {
        return ParseCompoundBlock(symbolTable, state);
    } else if (scanner.GetLexem().token == FOR) {
        return ParseForBlock(symTable, state);
    } else if (scanner.GetLexem().token == IDENTIFICATOR) {
        return ParseBlockIdent(symTable, state);
    }
}

Block* Parser::ParseCompoundBlock(SymbolTable* symbolTable, int state) {
    CheckNextLexem(scanner.GetLexem(), Lexem("begin", BEGIN));
    scanner.NextToken();
    BlockCompound* blockCompound = new BlockCompound();
    blockCompound->listBlock = ParseBlockList(symbolTable, state);
    CheckNextLexem(scanner.GetLexem(), Lexem("end", END));
    scanner.NextToken();
    CheckNextLexem(scanner.GetLexem(), Lexem(";", SEMI_COLON));
    scanner.NextToken();
    return blockCompound;
}

std::set<TokenType> endBlockToken = {END, ELSE, UNTIL, POINT};

std::vector<Block*> Parser::ParseBlockList(SymbolTable* symbolTable, int state) {
    std::vector<Block*> ans;
    while (endBlockToken.find(scanner.GetLexem().token) == endBlockToken.cend()) {
        Block* block = ParseBlock(symTable, state);
        if (block != nullptr) {
            ans.push_back(block);
        }
    }
    return ans;
}

Block* Parser::ParseForBlock(SymbolTable* symbolTable, int state) {
    scanner.NextToken();
    std::pair<int, int> pos = scanner.GetLexem().pos;
    Expression* exp1 = ParseExpression(0);
    bool toFlag;
    if (scanner.GetLexem().token == DOWNTO) {
        scanner.NextToken();
        toFlag = false;
    } else {
        CheckNextLexem(scanner.GetLexem(), Lexem("to", TO));
        toFlag = true;
    }
    pos = scanner.GetLexem().pos;
    scanner.NextToken();
    Expression* exp2 = ParseExpression(0);
    CheckNextLexem(scanner.GetLexem(), Lexem("do", DO));
    scanner.NextToken();
    Block* block = ParseBlock(symbolTable, state | 2);
//    scanner.NextToken();
    return new BlockFor(exp1, exp2, toFlag, block);
}

Block* Parser::ParseBlockIdent(SymbolTable* symTable, int state) {
    std::pair<int, int> pos = scanner.GetLexem().pos;
    Symbol* symbol = symTable->GetSymbol(scanner.GetLexem().val, pos);
    Expression* exp = ParseExpression(0);
    if (exp->expressionType == ExpressionType::ASSIGN) {
        CheckNextLexem(scanner.GetLexem(), Lexem(";", SEMI_COLON));
        scanner.NextToken();
        return new BlockAssign(exp);
    }
    if (exp->expressionType == ExpressionType::FUNCCALL) {
        CheckNextLexem(scanner.GetLexem(), Lexem(";", SEMI_COLON));
        scanner.NextToken();
        return new BlockFuncCall(exp);
    }
}

Block* Parser::ParseBlockStart() {
//    scanner.NextToken();
    this->ParseDeclaration(this->symTable);
    Block* block = new BlockCompound();
    CheckNextLexem(scanner.GetLexem(), Lexem("begin", BEGIN));
    scanner.NextToken();
    ((BlockCompound*)block)->listBlock = this->ParseBlockList(this->symTable, 0);
    scanner.NextToken();
    CheckNextLexem(scanner.GetLexem(), Lexem(".", POINT));
    return block;
}