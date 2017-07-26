#pragma once
#include "Interface.h"
#include <ostream>
#include <stack>

CFG_NAMESPACE_BEGIN

class EnumType;
class StructType;
class ScopeType;
class IFileData;

class CppExporter : protected IFileVisitor
{
public:
    static bool Export(std::ostream& stream, const ScopeType* global, const IFileData* fileData)
    {
        CppExporter cpp(stream, global);
        return cpp.Export(fileData);
    }

public:
    struct VarData
    {
        std::string type;
        std::string name;
        std::string value;
    };

protected:
    CppExporter(std::ostream& stream, const ScopeType* global);
    ~CppExporter();

protected:
    bool Export(const IFileData* file);

    bool Declare(const EnumType* ty, int tab);
    bool Declare(const StructType* ty, int tab);

    bool TabLoader(const StructType* ty, int tab);
    bool TabLoadStruct(const StructType* sType, const std::string& owner, int tab);
    bool TabLoadRaw(const IVariate* var, const std::string& name, int tab);
    bool TabLoadEnum(const IVariate* var, const std::string& name, int tab);
    bool TabLoadArray(const IVariate* var, const std::string& name, int tab);

    bool Enum2String(const EnumType* eType, int tab);
    bool String2Enum(const EnumType* eType, int tab);


    std::ostream& Tab(int tab);
protected:
    virtual void OnVariate(const IVariate* var);
    virtual void OnType(const IType* type);

protected:
    typedef std::stack<const IType*> StackScop;

    const ScopeType* _global;
    std::ostream& _stream;
    StackScop _scope;
    int _tab;
};

CFG_NAMESPACE_END
