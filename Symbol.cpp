#include "Symbol.h"
#include "Block.h"

SymbolTable::SymbolTable(SymbolTable * parentTable): parentTable(parentTable) {
//    symbols.push_back(new SymbolType("integer", DataType::INTEGER));
//    symbols.push_back(new SymbolType("real", DataType::REAL));
//    symbols.push_back(new SymbolType("char", DataType::CHAR));
//    symbols.push_back(new SymbolType("boolean", DataType::BOOLEAN));
//    symbols.push_back(new SymbolType("string", DataType::STRING));
//    symbols.push_back(new SymbolType("record", DataType::RECORD));
//    symbols.push_back(new SymbolType("pointer", DataType::POINTER));
//    stdTypeCount = symbols.size();
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
//        std::stringstream ss;
//        ss << "Unknown typename '" << name << "' (" << pos.first << ", " << pos.second <<").";
//        throw ParserException(ss.str());
        throw IdentNotFound(name, pos);
    }
    return temp->symbols[index];
}

void SymbolTable::GenerateVars(Generator* generator) {
    if (parentTable == nullptr) {
        for (auto it = symbols.begin() + stdTypeCount; it != symbols.end(); it++) {
            (*it)->Generate(generator);
            if ((*it)->declType == DeclarationType::VAR || (*it)->declType == DeclarationType::CONST) {
                ((SymbolIdent*)(*it))->localFlag = false;
            }
        }
    }
}

void SymbolTable::CheckLocalSymbol(std::string name, std::pair<int, int> pos) {
    if (FindSymbol(name) != -1) {
        //throw ParserException("Duplicate IDENTIFIER '" + name + "'");
        throw DuplicateIdent(name, pos);
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

Symbol* SymbolTable::FindReqSymbol(Expression* exp, std::pair<int, int> pos) {
    auto symbols = GetAllSymbols(((ExpressionIdent*)(((ExpressionFuncCall*)exp)->left))->symbol->name, pos);
    for (auto it: symbols) {
        if (it->declType != DeclarationType::FUNC && it->declType != DeclarationType::PROCEDURE) {
            continue;
        }
        if (it->declType == DeclarationType::FUNC) {
            if (((ExpressionFuncCall*)exp)->args.size() != ((SymbolFunction*)(it))->argc - 1) {
                continue;
            }

        }
        if (it->declType == DeclarationType::PROCEDURE) {
            if (((SymbolProcedure*)it)->argc == -1 || ((SymbolProcedure*)it)->argc == -2) {
                return it;
            }
            if (((ExpressionFuncCall*)exp)->args.size() != ((SymbolProcedure*)it)->argc) {
                continue;
            }
        }
        bool flag = false;
        for (auto arg = ((ExpressionFuncCall*)exp)->args.begin(); arg != ((ExpressionFuncCall*)exp)->args.end(); arg++) {
            auto temp = ((SymbolCall*)it)->symbolTable->symbols[arg - ((ExpressionFuncCall*)exp)->args.begin()];
            auto type = ((SymbolCall*)it)->symbolTable->symbols[arg - ((ExpressionFuncCall*)exp)->args.begin()]->GetType();
            if (type->declType == DeclarationType::RECORD && TypeChecker(this, std::make_pair(0, 0)).GetTypeID(*arg) == DataType::RECORD) {
                continue;
            }
            if (TypeChecker(this, std::make_pair(0 ,0)).GetTypeID(*arg) != ((SymbolType*)type)->dataType) {
                flag = true;
            }
        }

        if (flag) {
            continue;
        }
        return it;
    }
    throw IllegalExpression(pos);
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
    printIndent(spaces);
    std::cout << "Type" << indent << name << std::endl;
    if (type != nullptr) {
        type->Print(spaces + 1);
    }
}

void SymbolConst::Print(int spaces) {
    printIndent(spaces);
    std::cout << "Const" << indent << name << std::endl;
    if (type != nullptr) {
        type->Print(spaces + 1);
    }
    initExpr->Print(spaces + 1);
}

void SymbolVar::Print(int spaces) {
    printIndent(spaces);
    std::cout << "Var" << indent << name << std::endl;
    type->Print(spaces + 1);
    if (initExpr != nullptr) {
        initExpr->Print(spaces + 1);
    }
}

void SymbolPointer::Print(int spaces) {
    printIndent(spaces);
    std::cout << "Type" << indent << name << std::endl;
    type->Print(spaces + 1);
}

void SymbolDynArray::Print(int spaces) {
    printIndent(spaces);
    std::cout << "DynArray" << indent << name << std::endl;
    type->Print(spaces);
}

Symbol* SymbolDynArray::GetType() {
    return type;
}

void SymbolArray::Print(int spaces) {
    printIndent(spaces);
    std::cout << "Array" << indent << left << indent << right << std::endl;
    type->Print(spaces);
}

Symbol *SymbolArray::GetType() {
    return type;
}

Symbol* SymbolType::GetType() {
    return type;
}

int SymbolType::GetSize() {
    switch (dataType) {
        case DataType::INTEGER:
            return 4;
        case DataType::REAL:
            return 8;
        case DataType::CHAR:
        case DataType::BOOLEAN:
            return 1;
        default:
            if (type == nullptr) {
                return 0;
            }
            return type->GetSize();
    }
}

std::string SymbolType::GenerateName() {
    switch(dataType) {
        case DataType::INTEGER:
            return "dd";
        case DataType::CHAR:
            return "db";
        case DataType::BOOLEAN:
            return "db";
        case DataType::REAL:
            return "dq";
    }
}

Symbol* SymbolIdent::GetType() {
    return type;
}

std::string SymbolIdent::GenerateName() {
    return "v" + name;
}

std::string SymbolIdent::GetInitlist() {
    if (initExpr == nullptr) {
        if (((SymbolType*)type)->dataType == DataType::REAL) {
            return "0.0";
        } else {
            return "0";
        }
    }
    return initExpr->GenerateInitlist() +
            (initExpr->expressionType != ExpressionType::REAL && initExpr->typeID == DataType::REAL ? ".0" : "");
}

void SymbolIdent::Generate(Generator* generator) {
    generator->Add(GenerateName(), type->GenerateName(), GetInitlist());
}

int SymbolIdent::GetSize() {
    if (type == nullptr) {
        return 0;
    }
    return type->GetSize();
}

void SymbolRecord::Print(int spaces) {
    printIndent(spaces);
    std::cout << "Record" << std::endl;
    this->table->Print(spaces + 1);
    printIndent(spaces);
    std::cout << "End" << std::endl;
}

void SymbolFunction::Print(int spaces) {
    printIndent(spaces);
    std::cout << "Function" << indent << name << indent << argc << std::endl;
    symbolTable->Print(spaces + 1);
    block->Print(spaces + 1);
}

Symbol* SymbolFunction::GetType() {
    return type;
}

void SymbolProcedure::Print(int spaces) {
    printIndent(spaces);
    std::cout << "Procedure" << indent << name << indent << argc << std::endl;
    symbolTable->Print(spaces + 1);
    block->Print(spaces + 1);
}

void SymbolLabel::Print(int spaces) {
    printIndent(spaces);
    std::cout << "Label" << indent << name << std::endl;
}

void SymbolString::Print(int spaces) {
    printIndent(spaces);
    std::cout << "string" << indent;
    length != -1 ? std::cout << length << std::endl : std::cout << std::endl;
}
