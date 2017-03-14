#include "Parser.h"

const char fill = ' ';

void Parser::Print() {
    expression->Print(0);
}

void ExpressionBinOp::Print(int deep) {
    this->right->Print(deep + 1);
    std::string s;
    s += std::string(deep * 2, fill);
    std::cout << s << this->operation.val << std::endl;
    this->left->Print(deep + 1);
}

void ExpressionInteger::Print(int deep) {
    std::string s = std::string(deep * 2, fill);
    std::cout << s << this->val.val << std::endl;
}

void ExpressionReal::Print(int deep) {
    std::string s = std::string(deep * 2, fill);
    std::cout << s << this->val.val << std::endl;
}

void ExpressionIdent::Print(int deep) {
    std::string s = std::string(deep * 2, fill);
    std::cout << s << this->val.val << std::endl;
}

void ExpressionUnOp::Print(int deep) {
    this->arg->Print(deep + 1);
    std::string s = std::string(deep * 2, fill);
    std::cout << s << this->operation.val << std::endl;
}

Parser::Parser(char* filename): scanner(filename) {
    Lexem l;
    while (l.token != T_EOF) {
        scanner.NextToken();
        l = scanner.GetLexem();
        if (l.token == NUMBER || l.token == REAL_NUMBER || l.token == IDENTIFICATOR || l.token == OPEN_BRACKET ||
                l.token == ADD || l.token == SUB || l.token == TRUE || l.token == FALSE || l.token == NOT) {
            expression = ParseExpression();
            return;
        }
    }
}

Expression* Parser::ParseExpression() {
    auto left = ParseSimpleExpression();
    while (scanner.GetLexem().token == GREATER_THAN || scanner.GetLexem().token == GREATER_OR_EQUAL_THAN ||
            scanner.GetLexem().token == LESS_THAN || scanner.GetLexem().token == LESS_OR_EQUAL_THAN ||
            scanner.GetLexem().token == EQUAL || scanner.GetLexem().token == NOT_EQUAL) {
        Lexem operation = scanner.GetLexem();
        scanner.NextToken();
        if (scanner.GetLexem().token == OPEN_BRACKET) {
            scanner.NextToken();
            auto right = ParseExpression();
            if (scanner.GetLexem().token == CLOSE_BRACKET) {
                std::cout << "error";
                exit(0);
            }
            scanner.NextToken();
            if (scanner.GetLexem().token == ADD || scanner.GetLexem().token == SUB || scanner.GetLexem().token == OR ||
                    scanner.GetLexem().token == XOR) {
                auto operation2 = scanner.GetLexem();
                scanner.NextToken();
                left = (Expression*)new ExpressionBinOp(operation, left, (Expression*)new ExpressionBinOp(operation2, right, ParseSimpleExpression()));
            } else {
                left = (Expression*)new ExpressionBinOp(operation, left, right);
            }
        } else {
            auto right = ParseSimpleExpression();
            left = (Expression*)new ExpressionBinOp(operation, left, right);
        }
    }
    return left;
}

Expression* Parser::ParseSimpleExpression() {
    auto left = ParseTerm();
    while (scanner.GetLexem().token == ADD || scanner.GetLexem().token == SUB || scanner.GetLexem().token == OR ||
            scanner.GetLexem().token == XOR) {
        Lexem operation = scanner.GetLexem();
        scanner.NextToken();
        if (scanner.GetLexem().token == OPEN_BRACKET) {
            scanner.NextToken();
            auto right = ParseExpression();
            if (scanner.GetLexem().token != CLOSE_BRACKET) {
                std::cout << "error";
                exit(0);
            }
            scanner.NextToken();
            if (scanner.GetLexem().token == MULT || scanner.GetLexem().token == DIVISION || scanner.GetLexem().token == SHR ||
                scanner.GetLexem().token == AND || scanner.GetLexem().token == SHR || scanner.GetLexem().token == MOD ||
                    scanner.GetLexem().token == DIV) {
                auto operation2 = scanner.GetLexem();
                scanner.NextToken();
                left = (Expression*)new ExpressionBinOp(operation, left, (Expression*)new ExpressionBinOp(operation2, right, ParseTerm()));
            } else {
                left = (Expression*)new ExpressionBinOp(operation, left, right);
            }
        } else {
            auto right = ParseTerm();
            left = (Expression*)new ExpressionBinOp(operation, left, right);
        }
    }
    return left;
}

Expression* Parser::ParseTerm() {
    auto left = ParseFactor();
    while (scanner.GetLexem().token == MULT || scanner.GetLexem().token == DIVISION || scanner.GetLexem().token == SHR ||
            scanner.GetLexem().token == AND || scanner.GetLexem().token == SHR || scanner.GetLexem().token == MOD ||
            scanner.GetLexem().token == DIV) {
        Lexem operation = scanner.GetLexem();
        scanner.NextToken();
        if (scanner.GetLexem().token == OPEN_BRACKET) {
            scanner.NextToken();
            auto right = ParseExpression();
            if (scanner.GetLexem().token != CLOSE_BRACKET) {
                std::cout << "error";
                exit(0);
            }
            scanner.NextToken();
            left = (Expression*)new ExpressionBinOp(operation, left, right);
        } else {
            auto right = ParseFactor();
            left = (Expression*)new ExpressionBinOp(operation, left, right);
        }
    }
    return left;
}

Expression* Parser::ParseFactor() {
    auto lex = scanner.GetLexem();
    scanner.NextToken();
    if (lex.token == SUB || lex.token == ADD) {
        auto curExp = ParseExpression();
        return (Expression*)new ExpressionUnOp(lex, curExp);
    }
    if (lex.token == OPEN_BRACKET) {
        auto curExp = ParseExpression();
        if (scanner.GetLexem().token != CLOSE_BRACKET) {

        }
        scanner.NextToken();
        return curExp;
    }
    if (lex.token == NUMBER) {
        return (Expression*)new ExpressionInteger(lex);
    }
    if (lex.token == REAL_NUMBER) {
        return (Expression*)new ExpressionReal(lex);
    }
    if (lex.token == IDENTIFICATOR) {
        return (Expression*)new ExpressionIdent(lex);
    }
    if (lex.token == NOT) {
        auto tmp = ParseFactor();
        return (Expression*)new ExpressionUnOp(lex, tmp);
    }
    std::cout << "error";
    exit(0);
}
