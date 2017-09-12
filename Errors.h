#ifndef COMPILER_ERRORS_H_H
#define COMPILER_ERRORS_H_H
#include <string>
#include <iostream>

class ScannerException {
private:
    std::string msg;
public:
    ScannerException(std::string msg): msg(msg) {
        std::cout << msg << std::endl;
    }

    const std::string &getMsg() const {
        return msg;
    }
};

class ParserException {
private:
    std::string msg;
public:
    ParserException(std::string msg): msg(msg) {}

    const std::string &getMsg() const {
        return msg;
    }
};

class TypeCheckerException {
private:
    std::string msg;
public:
    TypeCheckerException(std::string type, std::pair<int, int> pos):
            msg("Incompatible types, expected \"" + type + "\" in pos "
            + "(" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + ")") {};

    const std::string &getMsg() const {
        return msg;
    }
};

static std::string StrPos(std::pair <int, int> pos) {
    return "Line " + std::to_string(pos.first) + " Column " + std::to_string(pos.second);
}

static std::string CheckSymbol(std::string str) {
    if (str.length() == 1) {
        if (str[0] < 0 || str[0] > 127) {
            return "EOF";
        }
    }
    return str;
}

class Error {
public:
    std::pair<int, int> pos;
    std::string errorMsg;
    Error(std::string errorMsg, std::pair<int, int> pos): errorMsg(errorMsg) {};
};

class DuplicateIdent: public Error {
public:
    DuplicateIdent(std::string name, std::pair<int, int> pos): Error("Duplicate identifier \"" + name + "\". " + StrPos(pos), pos) {};
};

class UnexpectedExpression: public Error {
public:
    UnexpectedExpression(std::string expected, std::string found, std::pair <int, int> pos):
    Error("Expression \"" + expected + "\" expected but \"" + CheckSymbol(found) + "\" found in " + StrPos(pos), pos) {};
};

class IllegalExpression: public Error {
public:
    IllegalExpression(std::pair<int, int> pos): Error("Illegal expression. " + StrPos(pos), pos) {};
};

class ExpectedConstExpression: public Error {
public:
    ExpectedConstExpression(std::pair<int, int> pos): Error("Const expression expected " + StrPos(pos), pos) {};
};

class TypeNotFound: public Error {
public:
    TypeNotFound(std::string name, std::pair<int, int> pos): Error("Unknown typename \"" + CheckSymbol(name) + "\" " + StrPos(pos), pos) {};
};

#endif //COMPILER_ERRORS_H
