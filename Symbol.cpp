#include "Symbol.h"
#include "Block.h"

SymbolTable::SymbolTable(SymbolTable * parentTable): parentTable(parentTable) {
    symbols.push_back(new SymbolType("integer", DataType::INTEGER));
    symbols.push_back(new SymbolType("real", DataType::REAL));
    symbols.push_back(new SymbolType("char", DataType::CHAR));
    symbols.push_back(new SymbolType("boolean", DataType::CHAR));
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
        if (symbols[i]->Same(name)) {
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
        std::stringstream ss;
        ss << "Unknown typename '" << name << "' (" << pos.first << ", " << pos.second <<").";
        throw ParserException(ss.str());
    }
    return temp->symbols[index];
}

void SymbolTable::CheckLocalSymbol(std::string name, std::pair<int, int> pos) {
    if (FindSymbol(name) != -1) {
        throw ParserException("Duplicate identificator '" + name + "'");
    }
}

bool SymbolTable::Find(std::string name) {
    SymbolTable* temp = this;
    do {
        for (auto symbol: temp->symbols) {
            if (symbol->Same(name)) {
                return true;
            }
        }
        temp = temp->parentTable;
    } while (temp != nullptr);
    return false;
}

std::vector<Symbol*> SymbolTable::GetAllSymbols(std::string name, std::pair<int, int> pos) {
    std::vector<Symbol*> ans;
    SymbolTable* tableNow = this;
    do {
        for (auto symbol: tableNow->symbols) {
            if (symbol->Same(name)) {
                ans.push_back(symbol);
            }
        }
        tableNow = tableNow->parentTable;
    } while (tableNow != nullptr);
    return ans;
}

Symbol* Symbol::GetType() {
    return nullptr;
}

bool Symbol::Same(std::string value) {
    return (name.length() == value.length()) && (strncasecmp(name.c_str(), value.c_str(), name.length()) == 0);
}

void SymbolType::Print(int spaces) {
    if (name.size() == 0) {
        return;
    }
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    };
    std::cout << "Type" << indent << name << std::endl;
    if (type != nullptr) {
        type->Print(spaces + 1);
    }
}

void SymbolConst::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    };
    std::cout << "Const" << indent << name << std::endl;
    if (type != nullptr) {
        type->Print(spaces + 1);
    }
    initExpr->Print(spaces + 1);
}

void SymbolVar::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "Var" << indent << name << std::endl;
    type->Print(spaces + 1);
    if (initExpr != nullptr) {
        initExpr->Print(spaces + 1);
    }
}

void SymbolPointer::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "Type" << indent << name << std::endl;
    type->Print(spaces + 1);
}

void SymbolArray::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "Array" << indent << std::endl;
    left->Print(spaces);
    right->Print(spaces);
    type->Print(spaces);
}

Symbol* SymbolType::GetType() {
    return type;
}

Symbol* SymbolIdent::GetType() {
    return type;
}

void SymbolRecord::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "Record" << std::endl;
    this->table->Print(spaces + 1);
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "End" << std::endl;
}

void SymbolFunction::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "Function" << indent << name << indent << argc << std::endl;
    symbolTable->Print(spaces + 1);
    block->Print(spaces + 1);
}

void SymbolProcedure::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "Procedure" << indent << name << indent << argc << std::endl;
    symbolTable->Print(spaces + 1);
    block->Print(spaces + 1);
}