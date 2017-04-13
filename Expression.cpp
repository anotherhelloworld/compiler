#include "Expression.h"

const char fill = ' ';
const int mult = 3;

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