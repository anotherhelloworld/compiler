#include "Expression.h"
#include "Symbol.h"

const char fill = ' ';
const int mult = 3;

std::set<TokenType> relations = {GREATER_THAN, GREATER_OR_EQUAL_THAN, LESS_THAN, LESS_OR_EQUAL_THAN, EQUAL, NOT_EQUAL};

std::map<TokenType, AsmTypeOperation> binOperators = {
        std::make_pair(ADD, AsmTypeOperation::ADD), std::make_pair(SUB, AsmTypeOperation::XOR), std::make_pair(OR, AsmTypeOperation::OR),
        std::make_pair(MULT, AsmTypeOperation::IMUL), std::make_pair(AND, AsmTypeOperation::AND)
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

void ExpressionBinOp::Generate(Generator* generator) {
    if (relations.find(operation.token) != relations.end()) {
        return;
    }
    left->Generate(generator);
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
        generator->Add(op, AsmTypeRegister::EAX, ((ExpressionInteger*)right)->val.val);
        generator->Add(AsmTypeOperation::PUSH, AsmTypeRegister::EAX);
        return;
    }
    right->Generate(generator);
    generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EBX);
    generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EAX);
    if (operation.token == DIV || operation.token == AND) {
        generator->Add(AsmTypeOperation::XOR, AsmTypeRegister::EDX, AsmTypeRegister::EAX);
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

void ExpressionFuncCall::Print(int spaces) {
    for (auto i: args) {
        i->Print(spaces + 1);
    }
    printIndent(spaces);
    std::cout << "()" << std::endl;
    left->Print(spaces + 1);
}

void ExpressionFuncCall::Generate(Generator* generator) {
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

    for (auto it = args.rbegin(); it != args.rend(); it++) {
        (*it)->Generate(generator);
        dataTypeExp.push_back(TypeChecker(((SymbolProcedure*)((ExpressionIdent*)left)->symbol)->symbolTable, std::make_pair(0, 0)).GetTypeID(*it));
    }

    std::string format = "\'";
    for (auto it = dataTypeExp.rbegin(); it != dataTypeExp.rend(); it++) {
        switch (*it) {
            case DataType::INTEGER:
                format += "%d";
                break;
            case DataType::REAL:
                format += "%f";
                break;
        }
    }
    generator->Add(AsmTypeOperation::PUSH, generator->AddFormat(format += argc == WRITELN ? "\', 10, 0" : "\', 0"));
    generator->Add(AsmTypeOperation::CALL, "printf");

    generator->Add(AsmTypeOperation::ADD, AsmTypeRegister::ESP, (int)pow(2, count) + 4);
};

void ExpressionPointer::Print(int spaces) {
    exp->Print(spaces + 1);
    std::cout << std::string(spaces * mult, fill) << "@" << std::endl;
}

void ExpressionDereference::Print(int spaces) {
    exp->Print(spaces + 1);
    std::cout << std::string(spaces * mult, fill) << "^" << std::endl;
}

void ExpressionInteger::Generate(Generator* generator) {
    generator->Add(AsmTypeOperation::PUSH, val.val);
}

int ExpressionInteger::GetSize() {
    return 4;
}