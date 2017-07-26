/*
 * 文件块
*/
#pragma once

#include "Interface.h"

CFG_NAMESPACE_BEGIN

class TypeBlock : public IBlock
{
public:
    TypeBlock(IType* type);
    ~TypeBlock() {}
public:
    virtual BlockCategory Category() const { return BlockCategory::Type; }
};

CFG_NAMESPACE_END
