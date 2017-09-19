#include "Expression.h"
#include "Symbol.h"

const char fill = ' ';
const int mult = 3;

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
        this->Generate()
    }
}

void ExpressionFuncCall::GenerateWrite(Generator* generator, int argc) {
    std::vector <DataType> dataTypeExp;
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
    generator->Add(AsmTypeOperation::PUSH, generator->AddFormat(format += argc == WRITELN ? "\', 0xA, 0x0" : "\', 0x0"));
    generator->Add(AsmTypeOperation::CALL, "printf");
    int size = 0;
    for (auto it = args.begin(); it != args.end(); it++) {
        size += (*it)->GetSize();
    }
};

void ExpressionPointer::Print(int spaces) {
    exp->Print(spaces + 1);
    std::cout << std::string(spaces * mult, fill) << "@" << std::endl;
}

void ExpressionDereference::Print(int spaces) {
    exp->Print(spaces + 1);
    std::cout << std::string(spaces * mult, fill) << "^" << std::endl;
}