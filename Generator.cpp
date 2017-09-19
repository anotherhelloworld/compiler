#include "Generator.h"

static std::string AsmOperationToString[] = {
        "", "push", "pop", "imul", "div", "add", "sub"
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

void Generator::Add(AsmTypeOperation operation, std::string val) {
    if (val[0] == '\'') {
        commands.push_back(new AsmCommandUnar(operation, new AsmStringConstant(val), (int)AsmCmdIndex::String));
    } else if (val[0] >= '0' && val[0] <= '9') {
        commands.push_back(new AsmCommandUnar(operation, new AsmIntConstant(val), (int)AsmCmdIndex::Integer));
    } else {
        commands.push_back(new AsmCommandUnar(operation, new AsmVar(val), (int)AsmCmdIndex::Ident));
    }
}

std::string Generator::AddFormat(std::string format) {
    std::string num = std::to_string((*frmtStr).size());
    (*frmtStr).push_back("format" + num + " : db" + format);
    return "format" + num;
}