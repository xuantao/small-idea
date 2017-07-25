/*
 * configuration file parser common define
 * 2017-07-22
*/
#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <type_traits>
#include <algorithm>

#define CFG_NAMESPACE       cfg
#define CFG_NAMESPACE_BEGIN namespace CFG_NAMESPACE {
#define CFG_NAMESPACE_END   }
#define CFG_NAMESPACE_USING using namespace CFG_NAMESPACE
#define CFG_NAMESPACE_REF   CFG_NAMESPACE::

CFG_NAMESPACE_BEGIN

// raw type name
#define TYPE_BOOL   "bool"
#define TYPE_INT    "int"
#define TYPE_FLOAT  "float"
#define TYPE_STRING "string"

enum class RawCategory
{
    Bool,
    Int,
    Float,
    String
};

enum class TypeCategory
{
    Raw,
    Enum,
    Struct,
    Array,
    Scope,
};

enum class ValueCategory
{
    Raw,
    Ref,
};


class IType;
class ITypeSet;
class IVarSet;
class IVariate;
class IValue;

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

CFG_NAMESPACE_END
