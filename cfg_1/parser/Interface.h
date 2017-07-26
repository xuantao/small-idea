/*
 * 各种基础接口类定义
*/
#pragma once

#include "CfgDef.h"

CFG_NAMESPACE_BEGIN

/*
 * 类型
*/
class IType
{
public:
    virtual ~IType() {}
public:
    virtual IType* Belong() const = 0;
    virtual TypeCategory Category() const = 0;
    virtual const std::string& Name() const = 0;

    virtual ITypeSet* TypeSet() const = 0;
    virtual IVarSet* VarSet() const = 0;
};

class ITypeSet
{
public:
    virtual ~ITypeSet() {}
public:
    virtual IType* Belong() const = 0;
    virtual IType* Get(const std::string& name) const = 0;
    virtual IType* Get(int index) const = 0;
    virtual int Size() const = 0;

    virtual bool Add(IType* type) = 0;
};

class IVarSet
{
public:
    virtual ~IVarSet() {}
public:
    virtual IType* Belong() const = 0;
    virtual IVariate* Get(const std::string& name) const = 0;
    virtual IVariate* Get(int index) const = 0;
    virtual int Size() const = 0;

    virtual bool Add(IVariate* var) = 0;
};

/*
 * 变量
*/
class IVariate
{
public:
    virtual ~IVariate() {}
public:
    virtual bool IsConst() const = 0;
    virtual const std::string& Name() const = 0;
    virtual const IType* Belong() const = 0;
    virtual const IType* Type() const = 0;
    virtual IValue* Value() const = 0;
    virtual void BindValue(IValue* value) = 0;
};

/*
 * 值
*/
class IValue
{
public:
    ~IValue() {}
public:
    virtual ValueCategory Category() const = 0;
};

/*
 * 文件块
*/
class IBlock
{
public:
    virtual ~IBlock() {}
public:
    virtual BlockCategory Category() const = 0;
};

/*
 * 文件数据
*/
class IFileData
{
public:
    virtual ~IFileData() {}

public:
    virtual const std::string& File() const = 0;
    virtual int Size() const = 0;
    virtual IBlock* Get(int index) const = 0;
    virtual void Traverse(IFileVisitor* visitor) const = 0;
};

class IFileVisitor
{
public:
    virtual ~IFileVisitor() {}

public:
    virtual void OnVariate(const IVariate* var) = 0;
    virtual void OnType(const IType* type) = 0;
};

CFG_NAMESPACE_END
