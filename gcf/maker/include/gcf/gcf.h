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
#include <functional>

#define GCF_NAMESPACE       gcf
#define GCF_NAMESPACE_BEGIN namespace GCF_NAMESPACE {
#define GCF_NAMESPACE_END   }
#define GCF_NAMESPACE_USING using namespace GCF_NAMESPACE
#define GCF_NAMESPACE_REF   GCF_NAMESPACE::

#define ERROR_NOT_ALLOW std::cerr << "not allow file:" << __FILE__ << " line:" << __LINE__ << std::endl

//#if defined(GCF_DLL_BUILD)
//    #if defined(_MSC_VER) || defined(__MINGW32__)
//        #define GCF_API __declspec(dllexport)
//    #endif // if defined(_MSC_VER)
//#elif defined(GCF_DLL)
//    #if defined(_MSC_VER) || defined(__MINGW32__)
//        #define GCF_API __declspec(dllimport)
//    #endif // if defined(_MSC_VER)
//#endif

#if !defined(GCF_API)
#define GCF_API
#endif

// raw type name
#define TYPE_BOOL   "bool"
#define TYPE_INT    "int"
#define TYPE_FLOAT  "float"
#define TYPE_STRING "string"

// file extend gcf = game configuration file
#define FILE_SUFFIX     ".gcf"
#define EXP_MODULE_API  "CreateExporter"

GCF_NAMESPACE_BEGIN

enum class ElementCategory
{
    Type,
    Var,
    Namespace,
    CrossCall,
};

enum class TypeCategory
{
    Raw,
    Enum,
    Struct,
    Array,
    Fucntion,
};

enum class RawCategory
{
    Bool,
    Int,
    Float,
    String
};

enum class ValueCategory
{
    Raw,
    Ref,
};

enum class BlockCategory
{
    Type,
    Var,
    CopySrc,
    Comment,
};

enum class CfgCategory
{
    None,
    Tab,
    Json,
};

class IType;
class IValue;
class IVariate;
class ITypeSet;
class IVarSet;
class INsSet;
class IScope;
class INamespace;

GCF_API class IElement
{
public:
    virtual ~IElement() {}
public:
    virtual ElementCategory ElementCat() const = 0;
};

/*
 * 类型
*/
GCF_API class IType : public IElement
{
public:
    virtual ~IType() {}
public:
    virtual TypeCategory TypeCat() const = 0;
    virtual const std::string& Name() const = 0;

    virtual IScope* Owner() const = 0;
    virtual IScope* Scope() const = 0;
};

/*
 * 原生类型
*/
GCF_API class IRawType : public IType
{
public:
    virtual ~IRawType() {}
public:
    virtual RawCategory RawCat() const = 0;
};

/*
 * 结构体类型
*/
GCF_API class IStructType : public IType
{
public:
    virtual ~IStructType() {}
public:
    virtual CfgCategory CfgCat() const = 0;
    virtual uint32_t HashCode() const = 0;
    virtual bool IsInherited(const IStructType* type) const = 0;
    virtual bool Inherit(IStructType* parent) = 0;
    virtual IStructType* Inherited() const = 0;

    virtual IScope* OwnScope() const = 0;
};

/*
* Enum Type
*/
GCF_API class IEnumType : public IType
{
public:
    virtual ~IEnumType() {}
};

/*
 * function
*/
GCF_API class IFunction : public IType
{
public:
    virtual ~IFunction() {}

public:
    virtual const std::string& RawName() const = 0;
    virtual uint32_t HashCode() const = 0;
    virtual IType* RetType() const = 0;
};

/*
 * 数组类型
*/
GCF_API class IArrayType : public IType
{
public:
    virtual ~IArrayType() {}
public:
    virtual IType* Original() const = 0;
    virtual IType* Prev() const = 0;
    virtual int Length() const = 0;
};

/*
 * namesapce
*/
GCF_API class INamespace : public IElement
{
public:
    virtual ~INamespace() {}
public:
    virtual const std::string& Name() const = 0;
    virtual IScope* Owner() const = 0;
    virtual IScope* Scope() const = 0;
};

/*
 * i need a name~
*/
GCF_API class ICrossCall : public IElement
{
public:
    virtual ~ICrossCall() {}
public:
    virtual const std::string& Name() const = 0;
    virtual uint32_t ID() const = 0;
    virtual IScope* Owner() const = 0;
    virtual IScope* Scope() const = 0;
};

/*
 * 变量
*/
GCF_API class IVariate : public IElement
{
public:
    virtual ~IVariate() {}
public:
    virtual bool IsConst() const = 0;
    virtual const std::string& Name() const = 0;
    virtual const std::string& Desc() const = 0;
    virtual IScope* Owner() const = 0;
    virtual IType* Type() const = 0;
    virtual IValue* Value() const = 0;
    virtual bool BindValue(IValue* value) = 0;
};

/*
 * 范围/域
*/
GCF_API class IScope
{
public:
    virtual ~IScope() {}

public:
    virtual const std::string& Name() const = 0;
    virtual IType* BindType() const = 0;
    virtual INamespace* BindNs() const = 0;
    virtual IScope* Owner() const = 0;

    virtual ITypeSet* TypeSet() const = 0;
    virtual IVarSet* VarSet() const = 0;
    virtual INsSet* NsSet() const = 0;

    virtual IElement* GetElement(const std::string& name) const = 0;
    virtual IScope* GetScope(const std::string& name) const = 0;
};

/*
 * type set
*/
GCF_API class ITypeSet
{
public:
    virtual ~ITypeSet() {}
public:
    virtual IType* Get(const std::string& name) const = 0;
    virtual IType* Get(int index) const = 0;
    virtual int Size() const = 0;

    virtual bool Add(IType* type) = 0;
    virtual bool Traverse(const std::function<bool(IType*)>& func) const = 0;
};

/*
 * variate set
*/
GCF_API class IVarSet
{
public:
    virtual ~IVarSet() {}
public:
    virtual IVariate* Get(const std::string& name) const = 0;
    virtual IVariate* Get(int index) const = 0;
    virtual int Size() const = 0;

    virtual bool Add(IVariate* var) = 0;
    virtual bool Traverse(const std::function<bool(IVariate*)>& func) const = 0;
};

/*
 * namespace set
*/
GCF_API class INsSet
{
public:
    virtual ~INsSet() {}
public:
    virtual INamespace* Get(const std::string& name) const = 0;
    virtual INamespace* Get(int index) const = 0;
    virtual int Size() const = 0;

    virtual bool Add(INamespace* ns) = 0;
    virtual bool Traverse(const std::function<bool(INamespace*)>& func) const = 0;
};

/*
 * 值
*/
GCF_API class IValue
{
public:
    virtual ~IValue() {}
public:
    virtual ValueCategory ValueCat() const = 0;
};

/*
 * 原生的数据
*/
GCF_API class IRawValue : public IValue
{
public:
    virtual ~IRawValue() {}
public:
    virtual RawCategory RawCat() const = 0;
    virtual bool AsBool() const = 0;
    virtual int AsInt() const = 0;
    virtual float AsFloat() const = 0;
    virtual const char* AsString() const = 0;
};

/*
 * 引用常量数据的值
*/
GCF_API class IRefValue : public IValue
{
public:
    virtual ~IRefValue() {}
public:
    virtual IVariate* Var() const = 0;
    virtual IRawValue* Original() const = 0;
};

/*
 * 导出器接口
*/
GCF_API class IExporter
{
public:
    virtual ~IExporter() {}

public:
    virtual void Release() = 0;

public:
    virtual bool OnBegin(const IScope* global, const char* path, const char* name) = 0;
    virtual void OnEnd() = 0;

    virtual void OnNsBegin(const std::string& name) = 0;
    virtual void OnNsEnd() = 0;

    virtual void OnInclude(const std::string& file) = 0;

    virtual void OnVariate(const IVariate* var) = 0;
    virtual void OnType(const IType* type) = 0;
    virtual void OnCrossCall(const ICrossCall* module) = 0;
};

typedef IExporter* (*CreateExporter)();

GCF_NAMESPACE_END
