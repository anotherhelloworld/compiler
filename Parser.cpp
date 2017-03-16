#include "Parser.h"

const char fill = ' ';

void Parser::Print() {
    expression->Print(0);
}

void ExpressionBinOp::Print(int deep) {
    this->right->Print(deep + 1);
    std::cout << std::string(deep, fill) << this->operation.val << std::endl;
    this->left->Print(deep + 1);
}

void ExpressionInteger::Print(int deep) {
    std::cout << std::string(deep, fill) << this->val.val << std::endl;
}

void ExpressionReal::Print(int deep) {
    std::cout << std::string(deep, fill) << this->val.val << std::endl;
}

void ExpressionIdent::Print(int deep) {
    std::cout << std::string(deep, fill) << this->val.val << std::endl;
}

void ExpressionUnOp::Print(int deep) {
    this->arg->Print(deep + 1);
    std::cout << std::string(deep, fill) << this->operation.val << std::endl;
}

void ExpressionArrayIndecies::Print(int deep) {
    this->ident->Print(deep + 1);
    std::cout << std::string(deep, fill) << this->operation.val << std::endl;
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
        if (scanner.GetLexem().token != CLOSE_BRACKET) {
            throw ParserException("Illegal expression: expected ')'");
        }
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
    throw ParserException("Illegal expression");
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
            if (lex.token != IDENTIFICATOR) {
                throw ParserException("Illegal expression: expected Identificator");
            }
            Expression* right = ParseTerm(false);
            res = (Expression*)new ExpressionBinOp(Lexem(".", POINT), res, right);
        } else if (lex.token == OPEN_SQUARE_BRACKET) {
            std::vector<Expression*> indecies = ParseArrayIndecies();
            res = (Expression*)new ExpressionArrayIndecies(Lexem("[", OPEN_SQUARE_BRACKET), res, indecies);
            lex = scanner.GetLexem();
            if (lex.token != CLOSE_SQUARE_BRACKET) {
                throw ParserException("Illegal expression: expected ]");
            }
        } else {
            flag = false;
        }
    }
    return res;
}

std::vector <Expression*> Parser::ParseArrayIndecies() {
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
