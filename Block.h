#ifndef COMPILER_BLOCK_H
#define COMPILER_BLOCK_H

#include <vector>
#include <iostream>
#include "Expression.h"

class Block {
public:
    virtual void Print(int);
};

class BlockCompound: public Block {
public:
    std::vector<Block*> listBlock;
    void Add(Block*);
    void Print(int);
};

class BlockCondition: public Block {
public:
    Expression* exp;
    BlockCondition(Expression* exp): exp(exp) {};
};

class BlockFor: public BlockCondition {
public:
    Expression* exp1;
    Expression* exp2;
    bool toFlag;
    Block* block;
    BlockFor(Expression* exp1, Expression* exp2, bool toFlag, Block* block):
            exp1(exp1), exp2(exp2), toFlag(toFlag), block(block),
            BlockCondition(new ExpressionBinOp(Lexem(toFlag ? ">" : "<", toFlag ? GREATER_THAN: LESS_THAN, std::make_pair(0, 0)), ((ExpressionAssign*)exp1)->left, exp2)) {};
    void Print(int);
};

class BlockFuncCall: public Block {
public:
    Expression* exp;
    BlockFuncCall(Expression* exp): exp(exp) {};
    void Print(int);
};

class BlockAssign: public Block {
public:
    Expression* exp;
    BlockAssign(Expression* exp): exp(exp) {};
    void Print(int);
};

class BlockWhile: public BlockCondition {
public:
    Block* block;
    BlockWhile(Expression* cond, Block* block): BlockCondition(cond), block(block) {};
    void Print(int);
};

class BlockContinue: public Block {
public:
    void Print(int);
};

class BlockIf: public BlockCondition {
public:
    Block* blockThen;
    Block* blockElse;
    BlockIf(Expression* exp, Block* blockThen, Block* blockElse): BlockCondition(exp), blockThen(blockThen), blockElse(blockElse) {}
    void Print(int);
};

class BlockRepeat: public BlockCondition {
public:
    std::vector<Block*> blocks;
    BlockRepeat(Expression* exp, std::vector<Block*> blocks): BlockCondition(exp), blocks(blocks) {};
    void Print(int);
};
#endif //COMPILER_BLOCK_H
