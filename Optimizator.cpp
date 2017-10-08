#include "Optimizator.h"

bool cmpCmdIndex(std::vector<AsmCommand*>::iterator it, int offset, AsmCmdIndex index) {
    auto temp = *(it + offset);
    return (*(it + offset))->index == (int)index;
}

void cutCmd(std::vector<AsmCommand*>* commands, std::vector<AsmCommand*>::iterator it, int count) {
    for (auto it2 = it; it2 < commands->end() - count; it2++) {
        *it2 = *(it2 + count);
    }
    for (int i = 0; i < count; ++i) {
        commands->pop_back();
    }
}

//pop eax
//cmd eax, ebx
//push eax

//cmd [esp + 4], ebx
bool template0(std::vector<AsmCommand*>* commands) {
    for (auto it = commands->begin(); it < commands->end() - 2; ++it) {
        if (cmpCmdIndex(it, 0, AsmCmdIndex::Register) &&
            cmpCmdIndex(it, 1, AsmCmdIndex::RegisterReg) &&
            cmpCmdIndex(it, 2, AsmCmdIndex::Register) &&
            (*it)->operation == AsmTypeOperation::POP && (*(it + 2))->operation == AsmTypeOperation::PUSH) {
            auto reg1 = (AsmTypeRegister*)((AsmCommandUnar*)*it)->operand;
            auto reg2 = (AsmTypeRegister*)((AsmCommandUnar*)*(it + 2))->operand;
            auto reg3 = (AsmTypeRegister*)((AsmCommandBinary*)*(it + 1))->operand1;
            if (reg1 == reg2 && reg1 == reg3) {
                ((AsmCommandBinary*)*(it + 1))->operand1 = new AsmAddress(AsmTypeRegister::ESP, 4);
                *it = *it + 1;
                cutCmd(commands, it + 1, 2);
                return true;
            }
        }
    }
    return false;
}

//pop reg
//push reg
bool template1(std::vector<AsmCommand*>* commands) {
    for (auto it = commands->begin(); it < commands->end() - 1; it++) {
        if ((*it)->index == (int)AsmCmdIndex::Lbael) {
            continue;
        }
        if ((*it)->operation == AsmTypeOperation::POP && (*(it + 1))->operation == AsmTypeOperation::PUSH) {
            if (cmpCmdIndex(it, 0, AsmCmdIndex::Register) && cmpCmdIndex(it, 1, AsmCmdIndex::Register)) {
                auto reg1 = ((AsmRegister*)((AsmCommandUnar*)*it)->operand)->reg;
                auto reg2 = ((AsmRegister*)((AsmCommandUnar*)*(it + 1))->operand)->reg;
                if (reg1 == reg2) {
                    cutCmd(commands, it, 2);
                    std::cout << "asd";
                    return true;
                }
            }
        }
    }
    return false;
}

//push reg1
//pop reg2
bool template2(std::vector<AsmCommand*>* commands) {
    for (auto it = commands->begin(); it < commands->end() - 1; it++) {
        if ((*it)->index == (int)AsmCmdIndex::Lbael) {
            continue;
        }
        if ((*it)->operation == AsmTypeOperation::PUSH && (*(it + 1))->operation == AsmTypeOperation::POP) {
            if (cmpCmdIndex(it, 0, AsmCmdIndex::Register) && cmpCmdIndex(it, 1, AsmCmdIndex::Register)) {
                auto reg1 = ((AsmRegister*)((AsmCommandUnar*)*it)->operand)->reg;
                auto reg2 = ((AsmRegister*)((AsmCommandUnar*)*(it + 1))->operand)->reg;
                if (reg1 == reg2) {
                    cutCmd(commands, it, 2);
                    return true;
                } else {
                    *it = new AsmCommandBinary(AsmTypeOperation::MOV, new AsmRegister(reg2), new AsmRegister(reg1), (int)AsmCmdIndex::RegisterReg);
                    cutCmd(commands, it + 1, 1);
                    return true;
                }
            }
        }
    }
    return false;
}

//mov reg, A
//push size

//push size [A]
bool template3(std::vector<AsmCommand*>* commands) {
    for (auto it = commands->begin(); it < commands->end(); it++) {
        if ((*it)->index == (int)AsmCmdIndex::Lbael) {
            continue;
        }
        if ((*it)->operation == AsmTypeOperation::MOV && (*(it + 1))->operation == AsmTypeOperation::PUSH) {
            if (cmpCmdIndex(it, 0, AsmCmdIndex::RegisterIdent) && cmpCmdIndex(it, 1, AsmCmdIndex::SizeAddrRegisterOffset)) {
                auto cmd1 = (AsmCommandBinary*)*it;
                auto cmd2 = (AsmAddress*)((AsmCommandUnarSize*)*(it + 1))->operand;
                if (((AsmRegister*)cmd1->operand1)->reg == ((AsmRegister*)cmd2->operand)->reg && cmd2->offset == 0) {
                    cmd2->operand = new AsmVar(*(AsmVar*)cmd1->operand2);
                    cutCmd(commands, it, 1);
                    return true;
                }
            }
        }
    }
    return false;
}

//mov reg, A
//push size [reg + A]
//push size [reg]
//
//push size [A + 4]
//push size [A]
bool template4(std::vector<AsmCommand*>* commands) {
    for (auto it = commands->begin(); it < commands->end() - 1; it++) {
        if ((*it)->index == (int)AsmCmdIndex::Lbael) {
            continue;
        }
        if ((*it)->operation == AsmTypeOperation::MOV && (*(it + 1))->operation == AsmTypeOperation::PUSH) {
            if (cmpCmdIndex(it, 0, AsmCmdIndex::RegisterIdent) &&
                cmpCmdIndex(it, 1, AsmCmdIndex::SizeAddrRegisterOffset) &&
                cmpCmdIndex(it, 2, AsmCmdIndex::SizeAddrRegisterOffset)) {
                auto cmd1 = (AsmCommandBinary*)*it;
                auto cmd2 = (AsmAddress*)((AsmCommandUnarSize*)*(it + 1))->operand;
                auto cmd3 = (AsmAddress*)((AsmCommandUnarSize*)*(it + 2))->operand;
                if (((AsmRegister*)cmd1->operand1)->reg == ((AsmRegister*)cmd2->operand)->reg && cmd2->offset == 4 && cmd3->offset == 0) {
                    cmd2->operand = new AsmVar(*(AsmVar*)cmd1->operand2);
                    cmd3->operand = new AsmVar(*(AsmVar*)cmd1->operand2);
                    cutCmd(commands, it, 1);
                    return true;
                }
            }
        }
    }
    return false;
}


bool template5(std::vector<AsmCommand*>* commands) {
    for (auto it = commands->begin(); it < commands->end() - 3; it++) {
        if ((*it)->index == (int)AsmCmdIndex::Lbael) {
            continue;
        }
        if ((*it)->operation == AsmTypeOperation::PUSH &&
           (*(it + 1))->operation == AsmTypeOperation::PUSH &&
           (*(it + 2))->operation == AsmTypeOperation::POP &&
           (*(it + 3))->operation == AsmTypeOperation::POP) {
            if (cmpCmdIndex(it, 0, AsmCmdIndex::Integer) &&
                cmpCmdIndex(it, 1, AsmCmdIndex::Integer) &&
                cmpCmdIndex(it, 2, AsmCmdIndex::Register) &&
                cmpCmdIndex(it, 3, AsmCmdIndex::Register)) {
                auto int1 = ((AsmIntConstant*)((AsmCommandUnar*)*it)->operand)->val;
                auto int2 = ((AsmIntConstant*)((AsmCommandUnar*)*(it + 1))->operand)->val;
                auto reg2 = ((AsmRegister*)((AsmCommandUnar*)*(it + 2))->operand)->reg;
                auto reg1 = ((AsmRegister*)((AsmCommandUnar*)*(it + 3))->operand)->reg;
                *it = new AsmCommandBinary(AsmTypeOperation::MOV, new AsmRegister(reg1), new AsmIntConstant(int1), (int)AsmCmdIndex::RegisterInt);
                *(it + 1) = new AsmCommandBinary(AsmTypeOperation::MOV, new AsmRegister(reg2), new AsmIntConstant(int2), (int)AsmCmdIndex::RegisterInt);
                cutCmd(commands, it + 2, 2);
                return true;
            }
        }
    }
    return false;
}

bool template6(std::vector<AsmCommand*>* commands) {
    for (auto it = commands->begin(); it < commands->end() - 1; it++) {
        if ((*it)->index == (int)AsmCmdIndex::Lbael) {
            continue;
        }
        if ((*it)->operation == AsmTypeOperation::PUSH && (*(it + 1))->operation == AsmTypeOperation::POP) {
            if (cmpCmdIndex(it, 0, AsmCmdIndex::Integer) && cmpCmdIndex(it, 1, AsmCmdIndex::Register)) {
                auto integer = ((AsmIntConstant*)((AsmCommandUnar*)*it)->operand)->val;
                auto reg = ((AsmRegister*)((AsmCommandUnar*)*(it + 1))->operand)->reg;
                *it = new AsmCommandBinary(AsmTypeOperation::MOV, new AsmRegister(reg), new AsmIntConstant(integer), (int)AsmCmdIndex::RegisterInt);
                cutCmd(commands, it + 1, 1);
                return true;
            }
        }
    }
    return false;
}

Optimizator::Optimizator(Generator* generator): generator(generator) {
    templates.push_back(&template0);
    templates.push_back(&template1);
    templates.push_back(&template2);//
    templates.push_back(&template3);
    templates.push_back(&template4);//
    templates.push_back(&template5);//
    templates.push_back(&template6);//
    
}

void Optimizator::OptimizeFunc(AsmFunction* function) {
    if (function->cmnds.size()) {
        OptimizeCode(&function->cmnds);
    }
    for (auto it : function->functions) {
        OptimizeFunc(it);
    }
}

void Optimizator::OptimizeCode(std::vector<AsmCommand*>* commands) {
    bool flag = true;
    do {
        flag = false;
        for (auto it : templates) {
            flag |= it(commands);
        }
    } while (flag);
}

void Optimizator::Optimize() {
    if (generator->commands.size()) {
        OptimizeCode(&generator->commands);
    }
    for (auto func : generator->functions) {
        OptimizeFunc(func);
    }
}