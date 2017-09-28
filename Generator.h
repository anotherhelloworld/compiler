#ifndef COMPILER_GENERATOR_H
#define COMPILER_GENERATOR_H
#include <vector>
#include <string>
#include <iostream>

enum class AsmTypeOperation {
    _NULL = 0, PUSH, POP, IMUL, DIV, ADD, SUB, NEG, NOT, OR, AND, XOR, SHL, SHR, CALL, MOV, RET, TEST, CMP, JZ, JNZ,
    JMP, JG, JGE, JL, JLE, JE, JNE, FLD, FILD, FSTP, FADD, FSUB, FDIV, FMUL, FIADD, FISUB, FIDIV, FIMUL, JA, JB, JAE,
    JBE, FCOM, FCOMI, FCOMIP
};

enum class AsmTypeRegister {
    EAX = 0, EBX, ECX, EDX, EBP, ESP, ST0, ST1
};

enum class AsmCmdIndex {
    Lbael = 0, Register, SizeAddrRegisterOffset, String, RegisterInt, RegisterIdent, AddrRegisterReg, Integer, Ident,
    CmdLabel, RegisterReg, RegisterAddrIdentOffset, RegisterString, StringReg, IntReg, IdentReg, SizeAddrIdentOffset,
    Cmd, AddrIdentOffset, AddrIdentOffsetRegister, RegisterAddrRegisterOffset, AddrRegisterOffsetRegister, SizeInt,
    SizeAddrRegisterRegister
};

enum class AsmTypeSize {
    QWORD, DWORD
};

enum class AsmTypeAddress {
    ADDR
};

enum class ArgTypeState {
    RVALUE, VAR, CONST, OUT
};

class AsmCommand {
public:
    AsmTypeOperation operation;
    int index;
    virtual std::string GetCode();
    AsmCommand(AsmTypeOperation operation, int index): operation(operation), index(index) {};
};

class AsmGlobalData {
public:
    std::string name;
    std::string type;
    std::string initList;
    std::string GetCode();
    AsmGlobalData(std::string name, std::string type, std::string initList): name(name), type(type), initList(initList) {};
};

class Generator {
public:
    Generator(): frmtStr(new std::vector<std::string>), constStr(new std::vector<std::string>), depth(0), maxDepth(0) {};
    void Add(AsmTypeOperation, AsmTypeRegister, int);
    void Add(AsmTypeOperation, std::string);
    void Add(AsmTypeOperation, AsmTypeRegister);
    void Add(AsmTypeOperation, AsmTypeRegister, std::string);
    void Add(AsmTypeOperation, AsmTypeRegister, AsmTypeRegister);
    void Add(std::string, std::string, std::string);
    void Add(AsmTypeOperation, AsmTypeSize, AsmTypeAddress, std::string, int);
    void Add(AsmTypeOperation, AsmTypeSize, AsmTypeAddress, AsmTypeRegister, int);
    void Add(AsmTypeOperation, AsmTypeAddress, AsmTypeRegister, int, AsmTypeRegister);
    void Add(AsmTypeOperation, AsmTypeRegister, AsmTypeAddress, std::string, int);
    void Add(AsmTypeOperation, AsmTypeRegister, AsmTypeAddress, AsmTypeRegister);
    void Add(AsmTypeOperation);
    void AddCallOffset(AsmTypeOperation, AsmTypeRegister, int, int);
    void AddLabel(std::string);
    std::string AddReal(std::string);
    std::string AddConstString(std::string);
    std::string AddFormat(std::string);
    std::string GetLocalLabel();
    void Print();
    void SaveLabels(std::string, std::string);
    void LoadLabels();

    std::vector <std::string>* frmtStr;
    std::vector <std::string>* constStr;
    std::vector <AsmCommand*> commands;
    std::vector <AsmGlobalData*> data;
    std::vector <std::string> breakLabels;
    std::vector <std::string> continueLabels;
    int depth;
    int maxDepth;
    int labelCount = 0;
    int realCount = 0;
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

class AsmCommandUnarSize: public AsmCommand {
public:
    AsmOperand* operand;
    AsmTypeSize size;
    AsmCommandUnarSize(AsmTypeOperation op, AsmTypeSize size, AsmOperand* operand, int cmdIndex):
            AsmCommand(op, cmdIndex), size(size), operand(operand) {};
    std::string GetCode();
};

class AsmAddress: public AsmOperand {
public:
    AsmOperand* operand;
    int offset;
    std::string GetCode();
    AsmAddress(std::string val, int offset): operand(new AsmVar(val)), offset(offset) {};
    AsmAddress(AsmTypeRegister reg, int offset): operand(new AsmRegister(reg)), offset(offset) {};
};

class AsmLabel: public AsmCommand {
public:
    std::string name;
    AsmLabel(std::string name): name(name), AsmCommand(AsmTypeOperation::_NULL, 0) {};
    std::string GetCode();
};

#endif //COMPILER_GENERATOR_H
