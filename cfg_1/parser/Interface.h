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

/*
 * 原生类型
*/
class IRawType : public IType
{
public:
    virtual ~IRawType() {}
public:
    virtual RawCategory Raw() const = 0;
};

/*
 * 自定义结构体类型
*/
class IStructType : public IType
{
public:
    virtual ~IStructType() {}
public:
    virtual bool IsInherited(const IType* type) const = 0;
    virtual IStructType* Inherited() const = 0;
    virtual IVarSet* OwnVars() const = 0;
};

/*
 * 数组类型
*/
class IArrayType : public IType
{
public:
    virtual ~IArrayType() {}
public:
    virtual IType* Original() const = 0;
    virtual IType* Prev() const = 0;
    virtual int Length() const = 0;
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
    virtual IType* Belong() const = 0;
    virtual IType* Type() const = 0;
    virtual IValue* Value() const = 0;
    virtual void BindValue(IValue* value) = 0;
};

/*
 * 值
*/
class IValue
{
public:
    virtual ~IValue() {}
public:
    virtual ValueCategory Category() const = 0;
};

/*
 * 原生的数据
*/
class IRawValue : public IValue
{
public:
    virtual ~IRawValue() {}
public:
    virtual RawCategory Raw() const = 0;
};

/*
 * 引用常量数据的值
*/
class IRefValue : public IValue
{
public:
    virtual ~IRefValue() {}
public:
    virtual IVariate* Var() const = 0;
    virtual IRawValue* Original() const = 0;
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
