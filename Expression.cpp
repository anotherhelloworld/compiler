#include "Expression.h"
#include "Symbol.h"

AsmTypeOperation SwitchCmd(DataType dataType, AsmTypeOperation cmdF, AsmTypeOperation cmdI) {
    return dataType == DataType::REAL ? cmdF : cmdI;
}

const char fill = ' ';
const int mult = 3;

std::set<TokenType> relations = {GREATER_THAN, GREATER_OR_EQUAL_THAN, LESS_THAN, LESS_OR_EQUAL_THAN, EQUAL, NOT_EQUAL};

std::map<TokenType, AsmTypeOperation> binOperators = {
        std::make_pair(ADD, AsmTypeOperation::ADD), std::make_pair(SUB, AsmTypeOperation::SUB), std::make_pair(XOR, AsmTypeOperation::XOR),
        std::make_pair(OR, AsmTypeOperation::OR), std::make_pair(MULT, AsmTypeOperation::IMUL), std::make_pair(AND, AsmTypeOperation::AND)
};

void printIndent(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    };
}

void ExpressionBinOp::Print(int deep) {
    this->right->Print(deep + 1);
    std::cout << std::string(deep * mult, fill) << this->operation.val << std::endl;
    this->left->Print(deep + 1);
}

void ExpressionTerm::Print(int deep) {
    std::cout << std::string(deep * mult, fill) << this->val.val << std::endl;
}

void ExpressionUnOp::Print(int deep) {
    this->arg->Print(deep + 1);
    std::cout << std::string(deep * mult, fill) << this->operation.val << std::endl;
}

void ExpressionArrayIndecies::Print(int deep) {
    this->ident->Print(deep + 1);
    std::cout << std::string(deep * mult, fill) << this->operation.val << std::endl;
    for (int i = 0; i < indecies.size(); i++) {
        this->indecies[i]->Print(deep + 1);
    }
}

void ExpressionInitializeList::Print(int spaces) {
    for (auto expr: initList) {
        expr->Print(spaces);
    }
}

void ExpressionTerm::GetIdentificitationList(ExpressionArgumentList* list) {
    list->arguments.push_back(val.val);
}

void ExpressionBinOp::GetIdentificitationList(ExpressionArgumentList* list) {
    right->GetIdentificitationList(list);
    left->GetIdentificitationList(list);
}

void ExpressionBinOp::GenerateDoubleExpr(Generator* generator) {
    left->Generate(generator);
    right->Generate(generator);
    if (right->typeID == DataType::REAL) {
        generator->Add(AsmTypeOperation::FLD, AsmTypeSize::QWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
        generator->Add(AsmTypeOperation::ADD, AsmTypeRegister::ESP, 8);
    } else {
        generator->Add(AsmTypeOperation::FILD, AsmTypeSize::DWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
        generator->Add(AsmTypeOperation::ADD, AsmTypeRegister::ESP, 4);
    }

    auto dataType = left->typeID;
    auto size = dataType == DataType::REAL ? AsmTypeSize::QWORD : AsmTypeSize::DWORD;
    switch (operation.token) {
        case ADD:
            generator->Add(SwitchCmd(dataType, AsmTypeOperation::FADD, AsmTypeOperation::FIADD), size, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
            break;
        case SUB:
            generator->Add(SwitchCmd(dataType, AsmTypeOperation::FSUB, AsmTypeOperation::FISUB), size, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
            break;
        case MULT:
            generator->Add(SwitchCmd(dataType, AsmTypeOperation::FMUL, AsmTypeOperation::FIMUL), size, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
            break;
        case DIVISION:
            generator->Add(SwitchCmd(dataType, AsmTypeOperation::FDIV, AsmTypeOperation::FIDIV), size, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
            break;
    }

    if (left->typeID != DataType::REAL) {
        generator->Add(AsmTypeOperation::SUB, AsmTypeRegister::ESP, 4);
    }
    generator->Add(AsmTypeOperation::FSTP, AsmTypeSize::QWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
}

void ExpressionBinOp::Generate(Generator* generator, ArgTypeState state) {
    if (relations.find(operation.token) != relations.end()) {
        GenerateRelations(generator, state);
        return;
    }
    if (typeID == DataType::REAL) {
        GenerateDoubleExpr(generator);
        return;
    }
    if (left->typeID == DataType::BOOLEAN && (operation.token == AND || operation.token == OR)) {
        if (operation.token == AND) {
            GenerateBoolExpr(generator, state, AsmTypeOperation::JZ);
        }
        if (operation.token == OR) {
            GenerateBoolExpr(generator, state, AsmTypeOperation::JNZ);
        }
        return;
    }

    right->Generate(generator);
    if (operation.token == SHL || operation.token == SHR) {
        generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EAX);
        if (right->expressionType != ExpressionType::INT) {
            throw ExpectedConstExpression(operation.pos);
        }
        AsmTypeOperation op;
        if (operation.token == SHL) {
            op = AsmTypeOperation::SHL;
        } else {
            op = AsmTypeOperation::SHR;
        }
        generator->Add(op, AsmTypeRegister::EAX, ((ExpressionInteger*)left)->val.val);
        generator->Add(AsmTypeOperation::PUSH, AsmTypeRegister::EAX);
        return;
    }
    left->Generate(generator);
    generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EBX);
    generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EAX);
    if (operation.token == DIV || operation.token == MOD) {
        generator->Add(AsmTypeOperation::XOR, AsmTypeRegister::EDX, AsmTypeRegister::EDX);
        generator->Add(AsmTypeOperation::DIV, AsmTypeRegister::EBX);
        if (operation.token == DIV) {
            generator->Add(AsmTypeOperation::PUSH, AsmTypeRegister::EAX);
        } else {
            generator->Add(AsmTypeOperation::PUSH, AsmTypeRegister::EDX);
        }
        return;
    }
    for (auto it = binOperators.begin(); it != binOperators.cend(); it++) {
        if ((*it).first == operation.token) {
            generator->Add((*it).second, AsmTypeRegister::EAX, AsmTypeRegister::EBX);
            generator->Add(AsmTypeOperation::PUSH, AsmTypeRegister::EAX);
            return;
        }
    }
}

void ExpressionBinOp::GenerateBoolExpr(Generator* generator, ArgTypeState state, AsmTypeOperation operation) {
    std::string label1 = generator->GetLocalLabel();
    std::string label2 = generator->GetLocalLabel();
    right->Generate(generator, state);
    generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EAX);
    generator->Add(AsmTypeOperation::TEST, AsmTypeRegister::EAX, AsmTypeRegister::EAX);
    generator->Add(operation, label1);
    right->Generate(generator, state);
    generator->Add(AsmTypeOperation::JMP, label2);
    generator->AddLabel(label1);
    generator->Add(AsmTypeOperation::PUSH, AsmTypeRegister::EAX);
    generator->AddLabel(label2);
}

void ExpressionBinOp::GenerateRelations(Generator* generator, ArgTypeState state) {
    std::string label1 = generator->GetLocalLabel();
    std::string label2 = generator->GetLocalLabel();
    right->Generate(generator, state);
    if (right->typeID != DataType::REAL && left->typeID == DataType::REAL) {
        generator->Add(AsmTypeOperation::FILD, AsmTypeSize::DWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
        generator->Add(AsmTypeOperation::SUB, AsmTypeRegister::ESP, 4);
        generator->Add(AsmTypeOperation::FSTP, AsmTypeSize::QWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
    }
    left->Generate(generator, state);
    if (right->typeID == DataType::REAL && left->typeID != DataType::REAL) {
        generator->Add(AsmTypeOperation::FILD, AsmTypeSize::DWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
        generator->Add(AsmTypeOperation::SUB, AsmTypeRegister::ESP, 4);
        generator->Add(AsmTypeOperation::FSTP, AsmTypeSize::QWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
    }

    auto dataType = right->typeID == DataType::REAL || left->typeID == DataType::REAL ? DataType::REAL : DataType::INTEGER;
    if (dataType == DataType::REAL) {
        generator->Add(AsmTypeOperation::FLD, AsmTypeSize::QWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
        generator->Add(AsmTypeOperation::FLD, AsmTypeSize::QWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 8);
        generator->Add(AsmTypeOperation::ADD, AsmTypeRegister::ESP, 16);
    }

    if (right->typeID == DataType::REAL || left->typeID == DataType::REAL) {
        generator->Add(AsmTypeOperation::FCOMIP, AsmTypeRegister::ST0, AsmTypeRegister::ST1);
        //generator->Add(AsmTypeOperation::FSTP);
    } else {
        generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EBX);
        generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EAX);
        generator->Add(AsmTypeOperation::CMP, AsmTypeRegister::EAX, AsmTypeRegister::EBX);
    }

    switch (operation.token) {
        case GREATER_THAN:
            generator->Add(SwitchCmd(dataType, AsmTypeOperation::JA, AsmTypeOperation::JG), label1);
            break;
        case GREATER_OR_EQUAL_THAN:
            generator->Add(SwitchCmd(dataType, AsmTypeOperation::JAE, AsmTypeOperation::JGE), label1);
            break;
        case LESS_THAN:
            generator->Add(SwitchCmd(dataType, AsmTypeOperation::JB, AsmTypeOperation::JL), label1);
            break;
        case LESS_OR_EQUAL_THAN:
            generator->Add(SwitchCmd(dataType, AsmTypeOperation::JBE, AsmTypeOperation::JLE), label1);
            break;
        case EQUAL:
            generator->Add(AsmTypeOperation::JE, label1);
            break;
        case NOT_EQUAL:
            generator->Add(AsmTypeOperation::JNE, label1);
            break;
    }
    generator->Add(AsmTypeOperation::PUSH, "0");
    generator->Add(AsmTypeOperation::JMP, label2);
    generator->AddLabel(label1);
    generator->Add(AsmTypeOperation::PUSH, "1");
    generator->AddLabel(label2);
}

int ExpressionBinOp::GetSize() {
    return std::max(left->GetSize(), right->GetSize());
}

void ExpressionUnOp::GetIdentificitationList(ExpressionArgumentList* list) {
    arg->GetIdentificitationList(list);
}

void ExpressionArrayIndecies::GetIdentificitationList(ExpressionArgumentList* list) {
    ident->GetIdentificitationList(list);
    for (int i = 0; i < indecies.size(); i++) {
        indecies[i]->GetIdentificitationList(list);
    }
}

void ExpressionAssign::Print(int spaces) {
    right->Print(spaces + 1);
    printIndent(spaces);
    std::cout << ":=" << std::endl;
    left->Print(spaces + 1);
}

void ExpressionAssign::Generate(Generator* generator, ArgTypeState state) {
    left->Generate(generator);
    right->Generate(generator, ArgTypeState::VAR);
    generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EAX);
    if (right->typeID == DataType::REAL && left->typeID != DataType::REAL) {
        left->ConvertToReal(generator);
    }
    int size = std::max(right->GetSize(), left->GetSize());
    for (int i = 0; i < size; i+=4) {
        generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EBX);
        generator->Add(AsmTypeOperation::MOV, AsmTypeAddress::ADDR, AsmTypeRegister::EAX, i, AsmTypeRegister::EBX);
    }
}

void ExpressionFuncCall::Print(int spaces) {
    for (auto i: args) {
        i->Print(spaces + 1);
    }
    printIndent(spaces);
    std::cout << "()" << std::endl;
    left->Print(spaces + 1);
}

void ExpressionFuncCall::Generate(Generator* generator, ArgTypeState state) {
    SymbolCall* leftSymbol = (SymbolCall*)((ExpressionIdent*)left)->symbol;
    int argc = leftSymbol->argc;
    if (argc == WRITE || argc == WRITELN) {
        this->GenerateWrite(generator, argc);
    }
}

void ExpressionFuncCall::GenerateWrite(Generator* generator, int argc) {
    std::vector <DataType> dataTypeExp;
    int size = 0;
    int count = 1;

    for (auto it = args.begin(); it != args.end(); it++) {
        size += (*it)->GetSize();
        count++;
    }


    int border = 16;
    int reserveStack = 8;
    while (size > border - reserveStack) {
        border += 16;
    }

    if (count > 1) {
        generator->Add(AsmTypeOperation::SUB, AsmTypeRegister::ESP, border - size - reserveStack);
    } else {
        generator->Add(AsmTypeOperation::SUB, AsmTypeRegister::ESP, border - size - reserveStack);
    }

    for (auto it = args.rbegin(); it != args.rend(); it++) {
        (*it)->Generate(generator);
        dataTypeExp.push_back(TypeChecker(((SymbolProcedure*)((ExpressionIdent*)left)->symbol)->symbolTable, std::make_pair(0, 0)).GetTypeID(*it));
    }

//    generator->Add(AsmTypeOperation::SUB, AsmTypeRegister::ESP, (int)pow(2, count));
//    int border = 16;
//    int reserveStack = 8;
//    while (size > border - reserveStack) {
//        border += 16;
//    }
//
//    if (count > 1) {
//        generator->AddCallOffset(AsmTypeOperation::SUB, AsmTypeRegister::ESP, border - size - reserveStack, count - 1);
//    } else {
//        generator->Add(AsmTypeOperation::SUB, AsmTypeRegister::ESP, border - size - 8);
//    }

    std::string format = "\'";
    for (auto it = dataTypeExp.rbegin(); it != dataTypeExp.rend(); it++) {
        switch (*it) {
            case DataType::INTEGER:
                format += "%d";
                break;
            case DataType::REAL:
                format += "%f";
                break;
            case DataType::CHAR:
                format += "%c";
                break;
            case DataType::STRING:
                format += "%s";
                break;
        }
    }

    generator->Add(AsmTypeOperation::PUSH, generator->AddFormat(format += argc == WRITELN ? "\', 10, 0" : "\', 0"));
    generator->Add(AsmTypeOperation::CALL, "_printf");

    generator->Add(AsmTypeOperation::ADD, AsmTypeRegister::ESP, border - 4);
};

void ExpressionPointer::Print(int spaces) {
    exp->Print(spaces + 1);
    std::cout << std::string(spaces * mult, fill) << "@" << std::endl;
}

void ExpressionDereference::Print(int spaces) {
    exp->Print(spaces + 1);
    std::cout << std::string(spaces * mult, fill) << "^" << std::endl;
}

void ExpressionInteger::Generate(Generator* generator, ArgTypeState state) {
    generator->Add(AsmTypeOperation::PUSH, val.val);
}

int ExpressionInteger::GetSize() {
    return 4;
}

void ExpressionInteger::ConvertToReal(Generator* generator) {
    generator->Add(AsmTypeOperation::FILD, AsmTypeSize::DWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
    generator->Add(AsmTypeOperation::SUB, AsmTypeRegister::ESP, 4);
    generator->Add(AsmTypeOperation::FSTP, AsmTypeSize::QWORD, AsmTypeAddress::ADDR, AsmTypeRegister::ESP, 0);
}

std::string ExpressionInteger::GenerateInitlist() {
    return val.val;
}

void ExpressionChar::Generate(Generator* generator, ArgTypeState state) {
    if (val.val.size() == 1) {
        generator->Add(AsmTypeOperation::PUSH, "\'" + val.val + "\'");
        return;
    }
    generator->Add(AsmTypeOperation::PUSH, generator->AddConstString(val.val));
}

int ExpressionChar::GetSize() {
    return 4;
}

void ExpressionReal::Generate(Generator* generator, ArgTypeState state) {
    std::string name = generator->AddReal(val.val);
    generator->Add(AsmTypeOperation::PUSH, AsmTypeSize::DWORD, AsmTypeAddress::ADDR, name, 4);
    generator->Add(AsmTypeOperation::PUSH, AsmTypeSize::DWORD, AsmTypeAddress::ADDR, name, 0);
}

int ExpressionReal::GetSize() {
    return 8;
}

std::string ExpressionReal::GenerateInitlist() {
    return val.val;
}

void ExpressionIdent::Generate(Generator* generator, ArgTypeState state) {
    auto idenSym = (SymbolIdent*)symbol;
    if (idenSym->localFlag) {
        if (idenSym->depth < generator->depth) {
            generator->Add(AsmTypeOperation::MOV, AsmTypeRegister::EAX, AsmTypeAddress::ADDR, "depth", 4 * idenSym->depth);
        } else {
            generator->Add(AsmTypeOperation::MOV, AsmTypeRegister::EAX, AsmTypeRegister::EBP);
        }
        generator->Add(AsmTypeOperation::ADD, AsmTypeRegister::EAX, idenSym->offset);
    } else {
        generator->Add(AsmTypeOperation::MOV, AsmTypeRegister::EAX, idenSym->GenerateName());
    }

    if ((idenSym->state == ArgTypeState::VAR || idenSym->state == ArgTypeState::CONST)) {
        generator->Add(AsmTypeOperation::MOV, AsmTypeRegister::EAX, AsmTypeAddress::ADDR, AsmTypeRegister::EAX);
    }

    if (state == ArgTypeState::RVALUE) {
        for (int i = idenSym->GetSize() - 4; i >= 0; i-=4) {
            generator->Add(AsmTypeOperation::PUSH, AsmTypeSize::DWORD, AsmTypeAddress::ADDR, AsmTypeRegister::EAX, i);
        }
        return;
    }

    generator->Add(AsmTypeOperation::PUSH, AsmTypeRegister::EAX);
}

int ExpressionIdent::GetSize() {
    return symbol->GetSize();
}

void ExpressionBoolean::Generate(Generator* generator, ArgTypeState state) {
    generator->Add(AsmTypeOperation::PUSH, std::strcmp(val.val.c_str(), "true") == 0 ? "1" : "0");
}

std::string ExpressionBoolean::GenerateInitList() {
    return std::strcmp(val.val.c_str(), "true") == 0 ? "1" : "0";
}