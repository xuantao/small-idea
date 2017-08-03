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
    virtual CfgCategory Cfg() const = 0;
    virtual bool IsInherited(const IType* type) const = 0;
    virtual IStructType* Inherited() const = 0;
    virtual IVarSet* OwnVars() const = 0;
};

/*
 * Enum Type
*/
class IEnumType : public IType
{
public:
    virtual ~IEnumType() {}
};

/*
 * Scope Type
*/
class IScopeType : public IType
{
public:
    virtual ~IScopeType() {}
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
    virtual const std::string& Desc() const = 0;
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

class ITabVisitor
{
public:
    virtual ~ITabVisitor() {}
public:
    virtual bool OnStart(const IStructType* sType) = 0;
    virtual bool OnEnd() = 0;
    virtual bool OnVar(const IVariate* var, const IRawType* rType,
        const std::string& title, const std::string& path) = 0;
    virtual bool OnVar(const IVariate* var, const IEnumType* eType,
        const std::string& title, const std::string& path) = 0;
    virtual bool OnVar(const IVariate* var, const IRawType* rType,
        const std::string& title, const std::string& path, int length) = 0;
    virtual bool OnVar(const IVariate* var, const IEnumType* eType,
        const std::string& title, const std::string& path, int length) = 0;
};

class IJsonVisitor
{
public:
    virtual ~IJsonVisitor() {}
public:
    virtual bool OnStart(const IStructType* sType) = 0;
    virtual bool OnEnd() = 0;
    //virtual bool OnVar(const IVariate* var, const IRawType* rType) = 0;
    //virtual bool OnVar(const IVariate* var, const IEnumType* eType) = 0;
    //virtual bool OnVar(const IVariate* var, const IStructType* eType) = 0;
    //virtual bool OnVar(const IVariate* var, const IRawType* rType, int length) = 0;
    //virtual bool OnVar(const IVariate* var, const IEnumType* eType, int length) = 0;
    //virtual bool OnVar(const IVariate* var, const IStructType* eType, int length) = 0;
};

/*
 * 导出器接口
*/
class IExporter
{
public:
    virtual ~IExporter() {}

public:
    virtual void OnBegin(const IScopeType* global, const std::string& file) = 0;
    virtual void OnEnd() = 0;

    virtual void OnNamespaceBegin(const std::string& name) = 0;
    virtual void OnNamesapceEnd() = 0;

    virtual void OnInclude(const std::string& file) = 0;

    virtual void OnVariate(const IVariate* var) = 0;
    virtual void OnType(const IType* type) = 0;
};

/*
 * tab creator
*/
class ITabCreater : public ITabVisitor
{
public:
    virtual ~ITabCreater() {}
public:
    virtual void SetPath(const std::string& path) = 0;
};

/*
 * json creator
*/
class IJsonCreater : public IJsonVisitor
{
public:
    virtual ~IJsonCreater() {}
public:
    virtual void SetPath(const std::string& path) = 0;
};
CFG_NAMESPACE_END
