#include "Block.h"
#include "Symbol.h"

void Block::Print(int) {};

void BlockCompound::Print(int spaces) {
    printIndent(spaces);
    std::cout << "begin" << std::endl;
    for (auto i: listBlock) {
        i->Print(spaces + 1);
    }
    printIndent(spaces);
    std::cout << "end" << std::endl;
};

void BlockCompound::Add(Block* block) {
    listBlock.push_back(block);
}

void BlockCompound::Generate(Generator* generator) {
    for (auto it = listBlock.begin(); it != listBlock.end(); it ++) {
        (*it)->Generate(generator);
    }
}

void BlockFor::Print(int spaces) {
    printIndent(spaces);
    std::cout << "for" << std::endl;
    exp1->Print(spaces + 1);
    printIndent(spaces);
    toFlag ? std::cout << "true" << std::endl : std::cout << "false" << std::endl;
    exp2->Print(spaces + 1);
    printIndent(spaces);
    std::cout << "do" << std::endl;
    if (block != nullptr) {
        block->Print(spaces + 1);
    }
}

void BlockFuncCall::Print(int spaces) {
    printIndent(spaces);
    std::cout << "Call" << std::endl;
    exp->Print(spaces + 1);
}

void BlockFuncCall::Generate(Generator* generator) {
    if (((ExpressionIdent*)((ExpressionFuncCall*)exp)->left)->symbol->declType == DeclarationType::FUNC) {
        exp->Generate(generator);
        SymbolFunction* sym = (SymbolFunction*)((ExpressionIdent*)((ExpressionFuncCall*)exp)->left)->symbol;
        generator->Add(AsmTypeOperation::ADD, AsmTypeRegister::ESP, ((SymbolIdent*)sym->symbolTable->symbols[sym->argc - 1])->type->GetSize());
    } else {
        exp->Generate(generator);
    }
}

void BlockAssign::Print(int spaces) {
    exp->Print(spaces);
}

void BlockAssign::Generate(Generator* generator) {
    exp->Generate(generator);
}

void BlockWhile::Print(int spaces) {
    printIndent(spaces);
    std::cout << "while" << std::endl;
    exp->Print(spaces + 1);
    printIndent(spaces);
    std::cout << "do" << std::endl;
    if (block != nullptr) {
        block->Print(spaces + 1);
    }
}

void BlockContinue::Print(int spaces) {
    printIndent(spaces);
    std::cout << "continue" << std::endl;
}

void BlockIf::Print(int spaces) {
    printIndent(spaces);
    std::cout << "if" << std::endl;
    exp->Print(spaces + 1);
    printIndent(spaces);
    std::cout << "then" << std::endl;
    blockThen->Print(spaces + 1);
    if (blockElse != nullptr) {
        printIndent(spaces);
        std::cout << "else" << std::endl;
        blockElse->Print(spaces + 1);
    }
}

void BlockIf::Generate(Generator* generator) {
    std::string labelNameElse = generator->GetLocalLabel();
    GenerateCond(generator);
    generator->Add(AsmTypeOperation::JZ, labelNameElse);
    blockThen->Generate(generator);
    generator->AddLabel(labelNameElse);
    if (blockElse != nullptr) {
        blockElse->Generate(generator);
    }
}

void BlockRepeat::Print(int spaces) {
    printIndent(spaces);
    std::cout << "until" << std::endl;
    exp->Print(spaces + 1);
    std::cout << "repeat" << std::endl;
    for (auto block: blocks) {
        block->Print(spaces + 1);
    }
}

void BlockGoTo::Print(int spaces) {
    printIndent(spaces);
    std::cout << "goto" << indent << labelSym->name << std::endl;
};

void BlockTryExcept::Print(int spaces) {
    printIndent(spaces);
    std::cout << "try" << std::endl;
    for (auto it = blockListTry.begin(); it != blockListTry.end(); it++) {
        (*it)->Print(spaces + 1);
    }
    printIndent(spaces);
    std::cout << "except" << std::endl;
    for (auto it = blockListExcept.begin(); it != blockListExcept.end(); it++) {
        (*it)->Print(spaces + 1);
    }
    printIndent(spaces);
    std::cout << "end" << std::endl;
} 

void BlockTryFinally::Print(int spaces) {
    printIndent(spaces);
    std::cout << "try" << std::endl;
    for (auto it = blockListTry.begin(); it != blockListTry.end(); it++) {
        (*it)->Print(spaces + 1);
    }
    printIndent(spaces);
    std::cout << "finally" << std::endl;
    for (auto it = blockListFinally.begin(); it != blockListFinally.end(); it++) {
        (*it)->Print(spaces + 1);
    }
    printIndent(spaces);
    std::cout << "end" << std::endl;
} 

void BlockBreak::Print(int spaces) {
    printIndent(spaces);
    std::cout << "break" << std::endl;
}

void BlockRaise::Print(int spaces) {
    printIndent(spaces + 1);
    std::cout << "raise" << std::endl;
    exp->Print(spaces + 1);
}

void BlockCase::Add(CaseNode caseNode) {
    caseList.push_back(caseNode);
}

void BlockCase::Print(int spaces) {
    printIndent(spaces);
    std::cout << "case" << std::endl;
    exp->Print(spaces);
    for (auto it = caseList.begin(); it != caseList.end(); it++) {
        (*it).block->Print(spaces + 1);
    }
}

void BlockCondition::GenerateCond(Generator* generator) {
    exp->Generate(generator);
    generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EAX);
    generator->Add(AsmTypeOperation::TEST, AsmTypeRegister::EAX, AsmTypeRegister::EAX);
}