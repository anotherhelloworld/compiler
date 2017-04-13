#include "Parser.h"

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
    while (lex.token != T_EOF) {
        if (lex.token == CONST) {
            ParseConstantDeclaration(symTable);
            break;
        }
        if (lex.token == TYPE) {
            ParseTypeDeclaration(symTable);
            break;
        }
        if (lex.token == VAR) {

        }
    }
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
        scanner.NextToken();
        Lexem lex = scanner.GetLexem();
        CheckNextLexem(lex, Lexem(":", COLON));
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
    if (scanner.GetLexem().token == STRING) {
        return ParseString(symTable);
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

Symbol* Parser::ParseString(SymbolTable* symTable) {
    scanner.NextToken();
    int length = -1;
    if (scanner.GetLexem().token == OPEN_SQUARE_BRACKET) {
        scanner.NextToken();
        std::pair <int, int> position = scanner.GetLexem().pos;
        Expression* length = ParseExpression(0);
        CheckNextLexem(scanner.GetLexem(), Lexem("]", CLOSE_SQUARE_BRACKET));
        scanner.NextToken();
    }
}

Expression* Parser::ParseInit(SymbolTable *) {
    scanner.NextToken();
    int count = 0;
    if (scanner.GetLexem().token == OPEN_SQUARE_BRACKET) {
        //something;
    }
    Expression* exp = ParseExpression(0);
    return exp;
}

Expression* Parser::ParseInitializeList(SymbolTable* symbolTable) {
    ExpressionInitializeList* res = new ExpressionInitializeList();
    do {
        scanner.NextToken();
        if (scanner.GetLexem().token == OPEN_BRACKET) {
            res->initList.push_back(ParseInitializeList(symbolTable));
            scanner.NextToken();
            CheckNextLexem(scanner.GetLexem(), Lexem(")", CLOSE_BRACKET));
            if (scanner.GetLexem().token != COMMA) {
//                return res;
            }
            continue;
        }
        Expression* exp = ParseExpression(0);
        CheckConstant(symbolTable, exp);
        res->initList.push_back(exp);
    } while (scanner.GetLexem().token == COMMA);
//    return res;
}

void Parser::CheckConstant(SymbolTable* symbolTable, Expression* expr) {
    ExpressionArgumentList* list = new ExpressionArgumentList();
    expr->GetIdentificitationList(list);
    if (list->flag == false) {
        throw;
    }
    for (auto symbol: list->arguments) {
        if (symbolTable->Find(symbol) != -1 && symbolTable->GetSymbol(symbol, scanner.GetLexem().pos)->declType != DeclarationType::CONST) {
            throw;
        }
    }
}
