#include "Generator.h"

static std::string AsmOperationToString[] = {
        "", "push", "pop", "imul", "div", "add", "sub", "neg", "not", "or", "and", "xor", "shl", "shr", "call", "mov"
};

static std::string AsmRegisterToString[] = {
        "eax", "ebx", "ecx", "edx", "ebp", "esp"
};

std::string AsmCommand::GetCode() {
    return AsmOperationToString[(int)operation];
}

std::string AsmCommandBinary::GetCode() {
    return AsmOperationToString[(int)operation] + " " + operand1->GetCode() + ", " + operand2->GetCode();
}

std::string AsmCommandUnar::GetCode() {
    return AsmOperationToString[(int)operation] + " " + operand->GetCode();
}

std::string AsmRegister::GetCode() {
    return AsmRegisterToString[(int)reg];
}

std::string AsmIntConstant::GetCode() {
    return val;
}

std::string AsmStringConstant::GetCode() {
    return str;
}

std::string AsmVar::GetCode() {
    return val;
}

void Generator::Add(AsmTypeOperation operation, AsmTypeRegister reg, int val) {
    commands.push_back(new AsmCommandBinary(operation, new AsmRegister(reg), new AsmIntConstant(std::to_string(val)), (int)AsmCmdIndex::RegisterInt));
}

void Generator::AddCallOffset(AsmTypeOperation operation, AsmTypeRegister reg, int val, int offset) {
    auto command = new AsmCommandBinary(operation, new AsmRegister(reg), new AsmIntConstant(std::to_string(val)), (int)AsmCmdIndex::RegisterInt);
    commands.insert(commands.end() - offset, command);
}

std::string Generator::AddConstString(std::string str) {
    std::string num = std::to_string((*constStr).size());
    (*constStr).push_back("str" + num + ": db \'" + str + "\', 0");
    return "str" + num;
}

void Generator::Add(AsmTypeOperation operation, std::string val) {
    if (val[0] == '\'') {
        commands.push_back(new AsmCommandUnar(operation, new AsmStringConstant(val), (int)AsmCmdIndex::String));
    } else if (val[0] >= '0' && val[0] <= '9') {
        commands.push_back(new AsmCommandUnar(operation, new AsmIntConstant(val), (int)AsmCmdIndex::Integer));
    } else {
        commands.push_back(new AsmCommandUnar(operation, new AsmVar(val), (int)AsmCmdIndex::Ident));
    }
}

void Generator::Add(AsmTypeOperation operation, AsmTypeRegister reg) {
    commands.push_back(new AsmCommandUnar(operation, new AsmRegister(reg), (int)AsmCmdIndex::Register));
}

void Generator::Add(AsmTypeOperation operation, AsmTypeRegister reg, std::string val) {
    if (val[0] == '\'') {
        commands.push_back(new AsmCommandBinary(operation, new AsmRegister(reg), new AsmStringConstant(val), (int)AsmCmdIndex::String));
    } else if (val[0] >= '0' && val[0] <= 9) {
        commands.push_back(new AsmCommandBinary(operation, new AsmRegister(reg), new AsmIntConstant(val), (int)AsmCmdIndex::Integer));
    } else {
        commands.push_back(new AsmCommandBinary(operation, new AsmRegister(reg), new AsmVar(val), (int)AsmCmdIndex::Ident));
    }
}

void Generator::Add(AsmTypeOperation operation, AsmTypeRegister reg1, AsmTypeRegister reg2) {
    commands.push_back(new AsmCommandBinary(operation, new AsmRegister(reg1), new AsmRegister(reg2), (int)AsmCmdIndex::Register));
}

std::string Generator::AddFormat(std::string format) {
    std::string num = std::to_string((*frmtStr).size());
    (*frmtStr).push_back("format" + num + " : db " + format);
    return "format" + num;
}

void Generator::Print() {
    std::cout << "bits 32" << std::endl;
    std::cout << "extern _printf" << std::endl;
    std::cout << "global _main" << std::endl;
    std::cout << "section .data" << std::endl;
    for (auto it : (*frmtStr)) {
        std::cout << "    " + it << std::endl;
    }
    std::cout << "section .text" << std::endl;
    std::cout << "_main:" << std::endl;
    for (auto it : commands) {
        std::cout << "    " << it->GetCode() << std::endl;
    }

    std::cout << "    mov eax, 0" << std::endl;
    std::cout << "    ret" << std::endl;
}