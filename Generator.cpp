#include "Generator.h"

static std::string AsmOperationToString[] = {
        "", "push", "pop", "imul", "div", "add", "sub"
};

std::string AsmCommand::GetCode() {
    return AsmOperationToString[(int)operation];
}

std::string AsmCommandBinary::GetCode() {
    return AsmOperationToString[(int)operation] + " " + operand1->GetCode() + ", " + operand2->GetCode();
}