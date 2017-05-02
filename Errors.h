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
    ParserException(std::string msg): msg(msg) {
//        std::cout << msg << std::endl;
    }

    const std::string &getMsg() const {
        return msg;
    }
};

//class ParserExceptionDecl: public ParserException {
//    ParserExceptionDecl(std::string msg): ParserException(msg) {};
//};

#endif //COMPILER_ERRORS_H
