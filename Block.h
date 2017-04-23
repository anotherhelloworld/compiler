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


#endif //COMPILER_BLOCK_H
