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
    TypeCheckerException(std::string type1, std::string type2, std::pair<int, int> pos):
            msg("Incompatible types, expected \"" + type1 + "\" but \"" + type2 + "\" found in pos "
            + "(" + std::to_string(pos.first) + ", " + std::to_string(pos.second) + ")") {};

    const std::string &getMsg() const {
        return msg;
    }
};
#endif //COMPILER_ERRORS_H
