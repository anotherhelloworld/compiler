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