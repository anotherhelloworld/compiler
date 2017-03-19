#include "Parser.h"

const char fill = ' ';

void Parser::Print() {
    expression->Print(0);
}

void ExpressionBinOp::Print(int deep) {
    this->right->Print(deep + 1);
    std::cout << std::string(deep * 3, fill) << this->operation.val << std::endl;
    this->left->Print(deep + 1);
}

void ExpressionTerm::Print(int deep) {
    std::cout << std::string(deep * 3, fill) << this->val.val << std::endl;
}

void ExpressionUnOp::Print(int deep) {
    this->arg->Print(deep + 1);
    std::cout << std::string(deep * 3, fill) << this->operation.val << std::endl;
}

void ExpressionArrayIndecies::Print(int deep) {
    this->ident->Print(deep + 1);
    std::cout << std::string(deep * 3, fill) << this->operation.val << std::endl;
    for (int i = 0; i < indecies.size(); i++) {
        this->indecies[i]->Print(deep + 1);
    }

}

std::vector <int> priorities(COUNT, -1);
std::vector <int> unarPriorities(COUNT, -1);

Parser::Parser(char* filename): scanner(filename) {
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

    expression = ParseExpression(0);
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
