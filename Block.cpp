#include "Block.h"

void Block::Print(int) {};

void BlockCompound::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "begin" << std::endl;
    for (auto i: listBlock) {
        i->Print(spaces + 1);
    }
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "end" << std::endl;
};

void BlockFor::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "for" << std::endl;
    exp1->Print(spaces + 1);
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    toFlag ? std::cout << "true" << std::endl : std::cout << "false" << std::endl;
    exp2->Print(spaces + 1);
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "do" << std::endl;
    if (block != nullptr) {
        block->Print(spaces + 1);
    }
}

void BlockFuncCall::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "Call" << std::endl;
    exp->Print(spaces + 1);
}

void BlockAssign::Print(int spaces) {
    exp->Print(spaces);
}

void BlockWhile::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "while" << std::endl;
    exp->Print(spaces + 1);
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "do" << std::endl;
    if (block != nullptr) {
        block->Print(spaces + 1);
    }
}

void BlockContinue::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "continue" << std::endl;
}

void BlockIf::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "if" << std::endl;
    exp->Print(spaces + 1);
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "then" << std::endl;
    blockThen->Print(spaces + 1);
    if (blockElse != nullptr) {
        for (int i = 0; i < spaces; i++) {
            std::cout << indent;
        }
        std::cout << "else" << std::endl;
        blockElse->Print(spaces + 1);
    }
}

void BlockRepeat::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << indent;
    }
    std::cout << "until" << std::endl;
    exp->Print(spaces + 1);
    std::cout << "repeat" << std::endl;
    for (auto block: blocks) {
        block->Print(spaces + 1);
    }
}