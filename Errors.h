#ifndef COMPILER_ERRORS_H_H
#define COMPILER_ERRORS_H_H
#include <string>
#include <iostream>

class ScannerException {
public:
    ScannerException(std::string msg) {
        std::cout << msg << std::endl;
    }
};

class ParserException {
public:
    ParserException(std::string msg) {
        std::cout << msg << std::endl;
    }
};

#endif //COMPILER_ERRORS_H
