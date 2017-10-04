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

void BlockFor::Generate(Generator* generator) {
    exp1->Generate(generator);
    std::string labelCond = generator->GetLocalLabel();
    std::string labelBreak = generator->GetLocalLabel();
    generator->SaveLabels(labelCond, labelBreak);
    generator->AddLabel(labelCond);
    GenerateCond(generator);
    generator->Add(AsmTypeOperation::JNZ, labelBreak);
    block->Generate(generator);
    ExpressionAssign(
            new ExpressionBinOp(
                    Lexem("", toFlag ? ADD : SUB),

                    ((ExpressionAssign*)exp1)->right,
                    new ExpressionInteger(Lexem("1", NUMBER))
            ),
            ((ExpressionAssign*)exp1)->right
    ).Generate(generator);
    generator->Add(AsmTypeOperation::JMP, labelCond);
    generator->AddLabel(labelBreak);
    generator->LoadLabels();
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

void BlockWhile::Generate(Generator* generator) {
    std::string labelCond = generator->GetLocalLabel();
    std::string labelBreak = generator->GetLocalLabel();
    generator->SaveLabels(labelCond, labelBreak);
    generator->AddLabel(labelCond);
    GenerateCond(generator);
    generator->Add(AsmTypeOperation::JZ, labelBreak);
    block->Generate(generator);
    generator->Add(AsmTypeOperation::JMP, labelCond);
    generator->AddLabel(labelBreak);
    generator->LoadLabels();
}

void BlockContinue::Print(int spaces) {
    printIndent(spaces);
    std::cout << "continue" << std::endl;
}

void BlockContinue::Generate(Generator* generator) {
    generator->Add(AsmTypeOperation::JMP, generator->GetLabelContinue());
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

void BlockRepeat::Generate(Generator* generator) {
    std::string labelBody = generator->GetLocalLabel();
    std::string labelBreak = generator->GetLocalLabel();
    std::string labelCond = generator->GetLocalLabel();
    generator->SaveLabels(labelCond, labelBreak);
    generator->AddLabel(labelBody);
    for (auto it : blocks) {
        it->Generate(generator);
    }
    generator->AddLabel(labelCond);
    GenerateCond(generator);
    generator->Add(AsmTypeOperation::JZ, labelBody);
    generator->AddLabel(labelBreak);
    generator->LoadLabels();
}

void BlockGoTo::Print(int spaces) {
    printIndent(spaces);
    std::cout << "goto" << indent << labelSym->name << std::endl;
};

void BlockGoToLabel::Print(int spaces) {
    printIndent(spaces);
    std::cout << "goToLabel" << indent << labelSym->name << std::endl;
}

void BlockGoToLabel::Generate(Generator* generator) {
    generator->AddLabel(generator->GetGlobalLabel(labelSym->name));
}

void BlockGoTo::Generate(Generator* generator) {
    generator->Add(AsmTypeOperation::JMP, generator->GetGlobalLabel(labelSym->name));
}

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

void BlockTryExcept::Generate(Generator* generator) {
    std::string oldErrorLabel = generator->errorLabel;
    std::string labelExcept = generator->GetGlobalLabel();
    generator->errorLabel = generator->GetNewErrorLabel();
    for (auto it : blockListTry) {
        it->Generate(generator);
    }
    generator->Add(AsmTypeOperation::JMP, labelExcept);
    generator->AddLabel(generator->errorLabel);
    for (auto it : blockListExcept) {
        it->Generate(generator);
    }
    generator->AddLabel(labelExcept);
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

void BlockTryFinally::Generate(Generator* generator) {
    std::string oldErrorLabel = generator->errorLabel;
    generator->errorLabel = generator->GetNewErrorLabel();
    for (auto it : blockListTry) {
        it->Generate(generator);
    }
    generator->AddLabel(generator->errorLabel);
    for (auto it : blockListFinally) {
        it->Generate(generator);
    }
}

void BlockBreak::Print(int spaces) {
    printIndent(spaces);
    std::cout << "break" << std::endl;
}

void BlockBreak::Generate(Generator* generator) {
    generator->Add(AsmTypeOperation::JMP, generator->GetLabelBreak());
}

void BlockRaise::Print(int spaces) {
    printIndent(spaces + 1);
    std::cout << "raise" << std::endl;
    exp->Print(spaces + 1);
}

void BlockRaise::Generate(Generator* generator) {
    std::string formatName = generator->AddFormat("\'%s\', 10, 0");
    generator->Add(AsmTypeOperation::SUB, AsmTypeRegister::ESP, 4);
    exp->Generate(generator);
    generator->Add(AsmTypeOperation::PUSH, formatName);
    generator->Add(AsmTypeOperation::CALL, "_printf");
    generator->Add(AsmTypeOperation::ADD, AsmTypeRegister::ESP, 12);
    generator->Add(AsmTypeOperation::JMP, generator->errorLabel);
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

void BlockCase::Generate(Generator* generator) {
    std::string labelBreak = generator->GetLocalLabel();
    for (auto it = caseList.begin(); it != caseList.end(); it++) {
        if (it->exp2 == nullptr) {
            ExpressionBinOp(Lexem("=", EQUAL), it->exp1, exp).Generate(generator);
            generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EAX);
        } else {
            ExpressionBinOp(Lexem(">=", GREATER_OR_EQUAL_THAN), exp, it->exp1).Generate(generator);
            ExpressionBinOp(Lexem("<=", LESS_OR_EQUAL_THAN), exp, it->exp2).Generate(generator);
            generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EAX);
            generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EBX);
            generator->Add(AsmTypeOperation::AND, AsmTypeRegister::EAX, AsmTypeRegister::EAX);
        }
        generator->Add(AsmTypeOperation::TEST, AsmTypeRegister::EAX, AsmTypeRegister::EAX);
        std::string labelElse = generator->GetLocalLabel();
        generator->Add(AsmTypeOperation::JZ, labelElse);
        it->block->Generate(generator);
        generator->Add(AsmTypeOperation::JMP, labelBreak);
        generator->AddLabel(labelElse);
    }
    if (blockElse != nullptr) {
        blockElse->Generate(generator);
    }
    generator->AddLabel(labelBreak);
}

void BlockCondition::GenerateCond(Generator* generator) {
    exp->Generate(generator);
    generator->Add(AsmTypeOperation::POP, AsmTypeRegister::EAX);
    generator->Add(AsmTypeOperation::TEST, AsmTypeRegister::EAX, AsmTypeRegister::EAX);
}