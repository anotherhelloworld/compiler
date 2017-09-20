#ifndef COMPILER_GENERATOR_H
#define COMPILER_GENERATOR_H
#include <vector>
#include <string>
#include <iostream>

enum class AsmTypeOperation {
    _NULL = 0, PUSH, POP, IMUL, DIV, ADD, SUB, NEG, NOT, OR, AND, XOR, SHL, SHR, CALL,
};

enum class AsmTypeRegister {
    EAX = 0, EBX, ECX, EDX, EBP, ESP
};

enum class AsmCmdIndex {
    Lbael = 0, Register, SizeAddrRegisterOffset, String, RegisterInt, RegisterIdent, AddrRegisterReg, Integer, Ident
};

enum class AsmSize {
    QWORD, DWORD
};

class AsmCommand {
public:
    AsmTypeOperation operation;
    int index;
    virtual std::string GetCode();
    AsmCommand(AsmTypeOperation operation, int index): operation(operation), index(index) {};
};

class Generator {
public:
    Generator(): frmtStr(new std::vector<std::string>), constStr(new std::vector<std::string>), depth(0), maxDepth(0) {};
    void Add(AsmTypeOperation, AsmTypeRegister, int);
    void Add(AsmTypeOperation, std::string);
    void Add(AsmTypeOperation, AsmTypeRegister);
    void Add(AsmTypeOperation, AsmTypeRegister, std::string);
    void Add(AsmTypeOperation, AsmTypeRegister, AsmTypeRegister);
    std::string AddFormat(std::string);
    void Print();

    std::vector <std::string>* frmtStr;
    std::vector <std::string>* constStr;
    std::vector <AsmCommand*> commands;
    int depth;
    int maxDepth;
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
    AsmCommandBinary(AsmTypeOperation operation, AsmOperand* operand1, AsmOperand* operand2, int index):
            AsmCommand(operation, index), operand1(operand1), operand2(operand2) {};
};

class AsmCommandUnar: public AsmCommand {
public:
    AsmOperand* operand;
    std::string GetCode();
    AsmCommandUnar(AsmTypeOperation operation, AsmOperand* operand, int index):
            AsmCommand(operation, index), operand(operand) {};
};

class AsmRegister: public AsmOperand {
public:
    AsmTypeRegister reg;
    std::string GetCode();
    AsmRegister(AsmTypeRegister reg): reg(reg) {};
};

class AsmIntConstant: public AsmOperand {
public:
    std::string val;
    std::string GetCode();
    AsmIntConstant(std::string val): val(val) {};
};

class AsmStringConstant: public AsmOperand {
public:
    std::string str;
    std::string GetCode();
    AsmStringConstant(std::string str): str(str) {};
};

class AsmVar: public AsmOperand {
public:
    std::string val;
    std::string GetCode();
    AsmVar(std::string val): val(val) {};
};

#endif //COMPILER_GENERATOR_H
