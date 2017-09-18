#ifndef COMPILER_GENERATOR_H
#define COMPILER_GENERATOR_H
#include <vector>
#include <string>

enum class AsmOperation {
    _NULL = 0, PUSH, POP, IMUL, DIV, ADD, SUB
};

enum class AsmRegister {
    EAX = 0, EBX, ECX, EDX, EBP, ESP
};

class Generator {
public:
    Generator(): frmtStr(new std::vector<std::string>), constStr(new std::vector<std::string>), depth(0), maxDepth(0) {};
    void Add(AsmOperation, AsmRegister, int);

    std::vector <std::string>* frmtStr;
    std::vector <std::string>* constStr;
    std::vector <AsmCommand*> commands;
    int depth;
    int maxDepth;
};

class AsmCommand {
public:
    AsmOperation operation;
    int index;
    virtual std::string GetCode();
    AsmCommand(AsmOperation operation, int index): operation(operation), index(index) {};
};

class AsmOperand {
public:
    virtual std::string GetCode() = 0;
};

class AsmCommandBinary: public AsmCommand {
public:
    AsmOperand* operand1;
    AsmOperand* operand2;
    std::string GetCode();
    AsmCommandBinary(AsmOperation op, AsmOperand* operand1, AsmOperand* operand2, int index):
            AsmCommand(operation, index), operand1(operand1), operand2(operand2) {};
    std::string GetCode();
};

class AsmRegister: public AsmOperand {
public:
    
};

#endif //COMPILER_GENERATOR_H
