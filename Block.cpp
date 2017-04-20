#include "Block.h"

void Block::Print(int) {};

void BlockCompound::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << "   ";
    }
    std::cout << "begin" << std::endl;
    for (auto i: listBlock) {
        i->Print(spaces + 1);
    }
    for (int i = 0; i < spaces; i++) {
        std::cout << "   ";
    }
    std::cout << "end" << std::endl;
};

void BlockFor::Print(int spaces) {
    for (int i = 0; i < spaces; i++) {
        std::cout << "   ";
    }
    std::cout << "for" << std::endl;
    exp1->Print(spaces + 1);
    for (int i = 0; i < spaces; i++) {
        std::cout << "   ";
    }
    toFlag ? std::cout << "true" << std::endl : std::cout << "false" << std::endl;
    exp2->Print(spaces + 1);
    for (int i = 0; i < spaces; i++) {
        std::cout << "   ";
    }
    std::cout << "do" << std::endl;
    if (block != nullptr) {
        block->Print(spaces + 1);
    }
}