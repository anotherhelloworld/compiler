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

// class CompilerError {
// public:
//     std::pair <int, int> pos;
//     std::string errorMsg;
//     CompilerError(std::string errorMsg, std::pair<int, int> pos): errorMsg(errorMsg) {};
// };

// class TypeError: public CompilerError {
// public:
//     TypeError(std::string expectedType, std::pair<int, int> pos): TypeError("")
// };
#endif //COMPILER_ERRORS_H
