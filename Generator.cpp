#include "Generator.h"

static std::string AsmOperationToString[] = {
        "", "push", "pop", "imul", "div", "add", "sub", "neg", "not", "or", "and", "xor", "shl", "shr", "call", "mov",
        "ret", "test", "cmp", "jz", "jnz", "jmp", "jg", "jge", "jl", "jle", "je", "jne", "fld", "fild", "fstp", "fadd",
        "fsub", "fdiv", "fmul", "fiadd", "fisub", "fidiv", "fimul", "ja", "jb", "jae", "jbe", "fcom", "fcomi", "fcomip"
};

static std::string AsmRegisterToString[] = {
        "eax", "ebx", "ecx", "edx", "ebp", "esp", "ST0", "ST1"
};

static std::string AsmSizeToString[] = {
        "qword", "dword"
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

void Generator::AddLabel(std::string name) {
    commands.push_back(new AsmLabel(name));
}

std::string Generator::AddReal(std::string val) {
    realCount++;
    std::string name = "d" + std::to_string(realCount);
    Add(name, "dq", val);
    return name;
}

std::string Generator::GetLocalLabel() {
    return ".L" + std::to_string(++labelCount);
}

std::string Generator::GetLabelBreak() {
    return *breakLabels.crbegin();
}

std::string Generator::GetLabelContinue() {
    return *continueLabels.crbegin();
}

std::string Generator::GetGlobalLabel(std::string name) {
    return "L" + name;
}

std::string Generator::GetGlobalLabel() {
    return "G" + std::to_string(labelCount);
}

std::string Generator::GetNewErrorLabel() {
    return "E" + std::to_string(errorCount++);
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
    commands.push_back(new AsmCommandBinary(operation, new AsmRegister(reg1), new AsmRegister(reg2), (int)AsmCmdIndex::RegisterReg));
}

void Generator::Add(std::string name, std::string type, std::string initList) {
    data.push_back(new AsmGlobalData(name, type, initList));
}

void Generator::Add(AsmTypeOperation op, AsmTypeSize size, AsmTypeAddress addres, std::string val, int offset) {
    commands.push_back(new AsmCommandUnarSize(op, size, new AsmAddress(val, offset), (int)AsmCmdIndex::SizeAddrIdentOffset));
}

void Generator::Add(AsmTypeOperation op, AsmTypeSize size, AsmTypeAddress addres, AsmTypeRegister reg, int offset) {
    commands.push_back(new AsmCommandUnarSize(op, size, new AsmAddress(reg, offset), (int)AsmCmdIndex::SizeAddrRegisterOffset));
};

void Generator::Add(AsmTypeOperation op, AsmTypeAddress addr, AsmTypeRegister reg1, int offset, AsmTypeRegister reg2) {
    commands.push_back(new AsmCommandBinary(op, new AsmAddress(reg1, offset), new AsmRegister(reg2), (int)AsmCmdIndex::AddrRegisterOffsetRegister));
}

void Generator::Add(AsmTypeOperation op, AsmTypeRegister reg, AsmTypeAddress addr, std::string var, int offset) {
    commands.push_back(new AsmCommandBinary(op, new AsmRegister(reg), new AsmAddress(var, offset), (int)AsmCmdIndex::RegisterAddrIdentOffset));
}

void Generator::Add(AsmTypeOperation op, AsmTypeRegister reg1, AsmTypeAddress addr, AsmTypeRegister reg2) {
    commands.push_back(new AsmCommandBinary(op, new AsmRegister(reg1), new AsmAddress(reg2, 0), (int)AsmCmdIndex::RegisterAddrRegisterOffset));
}

void Generator::Add(AsmTypeOperation op) {
    commands.push_back(new AsmCommand(op, (int)AsmCmdIndex::Cmd));
}

void Generator::Add(AsmFunction* function) {
    functions.push_back(function);
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
    std::cout << "    depth: times " + std::to_string(maxDepth * 4) + " dd 0" << std::endl;
    for (auto it : (*frmtStr)) {
        std::cout << "    " + it << std::endl;
    }

    for (auto it : (*constStr)) {
        std::cout << "    " + it << std::endl;
    }

    for (auto it : data) {
        std::cout << "    " + (*it).GetCode() << std::endl;
    }
    std::cout << "section .text" << std::endl;
    for (auto it : functions) {
        std::cout << it->GetCode() << std::endl;
    }
    std::cout << "_main:" << std::endl;
    for (auto it : commands) {
        std::cout << "    " << it->GetCode() << std::endl;
    }

    std::cout << "    mov eax, 0" << std::endl;
    std::cout << "    ret" << std::endl;
}

void Generator::SaveLabels(std::string labelContinue, std::string labelBreak) {
    breakLabels.push_back(labelBreak);
    continueLabels.push_back(labelContinue);
}

void Generator::LoadLabels() {
    breakLabels.pop_back();
    continueLabels.pop_back();
}

std::string AsmGlobalData::GetCode() {
    return name + ": " + type + " " + initList;
}

std::string AsmCommandUnarSize::GetCode() {
    return AsmOperationToString[(int)operation] + " " + AsmSizeToString[(int)size] + " " + operand->GetCode();
}

std::string AsmAddress::GetCode() {
    return "[" + operand->GetCode() + (offset == 0 ? "" : " + " + std::to_string(offset)) + "]";
}

std::string AsmLabel::GetCode() {
    return name + ": ";
}

std::string AsmFunction::GetCode() {
    std::string ans;
    ans = name + ":\n";
    for (int i = 0; i < cmnds.size(); i++) {
        ans += "    " + cmnds[i]->GetCode() + "\n";
    }
    for (int i = 0; i < this->functions.size(); i++) {
        ans += functions[i]->GetCode() + "\n";
    }
    return ans;
}