#include "Block.h"

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

void BlockAssign::Print(int spaces) {
    exp->Print(spaces);
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

void BlockRepeat::Print(int spaces) {
    printIndent(spaces);
    std::cout << "until" << std::endl;
    exp->Print(spaces + 1);
    std::cout << "repeat" << std::endl;
    for (auto block: blocks) {
        block->Print(spaces + 1);
    }
}