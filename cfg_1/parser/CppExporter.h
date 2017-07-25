#pragma once
#include "CfgDef.h"
#include <ostream>
#include <stack>

CFG_NAMESPACE_BEGIN

class EnumType;
class StructType;
class ScopeType;

class CppExporter
{
public:
    static bool Export(const ScopeType* global, std::ostream& stream)
    {
        CppExporter cpp(global, stream);
        return cpp.Export();
    }

public:
    struct VarData
    {
        std::string type;
        std::string name;
        std::string value;
    };

protected:
    CppExporter(const ScopeType* global, std::ostream& stream);
    ~CppExporter();

protected:
    bool Export();
    //bool Export(const IType* ty);
    //bool Export(const IVariate* var);
    bool Export(const EnumType* ty);
    bool Export(const StructType* ty);
    bool Export(const ScopeType* ty);
    void Trans(const IVariate* var, VarData& data);
    void Tab();

protected:
    typedef std::stack<const IType*> StackScop;

    const ScopeType* _global;
    std::ostream& _stream;
    StackScop _scope;
    int _tab;
};

CFG_NAMESPACE_END
