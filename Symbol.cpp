#include "Symbol.h"

SymbolTable::SymbolTable(SymbolTable * parentTable): parentTable(parentTable) {
    symbols.push_back(new SymbolType("integer", DataType::INTEGER));
    symbols.push_back(new SymbolType("double", DataType::DOUBLE));
    stdTypeCount = symbols.size();
};

void SymbolTable::Print(int spaces) {
    for (int i = 0 + stdTypeCount; i < symbols.size(); i++) {
        symbols[i]->Print(spaces);
        std::cout << std::endl;
    }
}

void SymbolTable::Add(Symbol* symbol) {
    symbols.push_back(symbol);
}

int SymbolTable::FindSymbol(std::string name) {
    for (int i = 0; i < symbols.size(); i++) {
        if (symbols[i]->name.length() == name.length() && strncasecmp(symbols[i]->name.c_str(), name.c_str(), symbols[i]->name.length()) == 0) {
            return i;
        }
    }
    return -1;
}

Symbol* SymbolTable::GetSymbol(std::string name, std::pair<int, int> pos) {
    int index = -1;
    SymbolTable* temp = this;
    do {
        index = temp->FindSymbol(name);
        if (index != -1) {
            break;
        }
        temp = temp->parentTable;
    } while (temp != nullptr);
    if (index == -1) {

    }
    return temp->symbols[index];
}

void SymbolTable::CheckLocalSymbol(std::string name, std::pair<int, int> pos) {
    if (FindSymbol(name) != -1) {
        throw;
    }
}

bool SymbolTable::Find(std::string name) {
    SymbolTable* temp = this;
    do {
        for (auto symbol: temp->symbols) {
            if (symbol->name.length() == name.length() && strncasecmp(symbol->name.c_str(), name.c_str(), symbol->name.length()) == 0) {
                return true;
            }
        }
        temp = temp->parentTable;
    } while (temp != nullptr);
    return false;
}

Symbol* Symbol::GetType() {
    return nullptr;
}

void SymbolType::Print(int spaces) {
    if (name.size() == 0) {
        return;
    }
    for (int i = 0; i < spaces; i++) {
        std::cout << "   ";
    };
    std::cout << "Type" << "   " << name << std::endl;
    if (type != nullptr) {
        type->Print(spaces + 1);
    }
}

void SymbolConst::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << " ";
    };
    std::cout << "Const" << "   " << name << std::endl;
    if (type != nullptr) {
        type->Print(spaces + 1);
    }
    initExpr->Print(spaces + 1);
}

Symbol* SymbolType::GetType() {
    return type;
}

Symbol* SymbolIdent::GetType() {
    return type;
}