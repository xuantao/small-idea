#pragma once
#include "CfgDef.h"
#include <ostream>
#include <functional>

CFG_NAMESPACE_BEGIN

class IStructType;
class ITabVisitor;

struct UniqueVarName
{
    UniqueVarName()
    {

    }

    UniqueVarName(const IVariate* _var, const std::string& _name)
        : var(_var), name(_name)
    {
    }

    const IVariate* var;
    std::string name;
};

namespace utility
{
    static const std::string EMPTY_STR;
    static const std::vector<std::string> EMPTY_VEC_STR;

    bool Convert(const std::string& str, bool& out);
    bool Convert(const std::string& str, bool& out, bool def);

    bool Convert(const std::string& str, int& out);
    bool Convert(const std::string& str, int& out, int def);

    bool Convert(const std::string& str, float& out);
    bool Convert(const std::string& str, float& out, float def);

    std::string TrimFileSuffix(const std::string& file, char c = '.');
    std::string TrimLeft(const std::string& str, const std::string& trim);
    std::string TrimRight(const std::string& str, const std::string& trim);
    std::string Trim(const std::string& str, const std::string& trim);
    std::string Replace(const std::string& str, const std::string& _r, const std::string& _n);

    std::string Contact(const std::vector<std::string>& path, const std::string& c);
    std::vector<std::string> Split(const std::string& str, const std::string& s);

    std::vector<std::string> Absolute(const IType* type);
    std::vector<std::string> Relative(const IType* self, const IType* other);

    IType* FindType(IType* scope, const std::string& path);
    IVariate* FindVar(IType* scope, const std::string& path);

    std::ostream& Tab(std::ostream& stream, int tab);

    std::vector<UniqueVarName> UniqueName(const IStructType* sType, const std::string& owner = EMPTY_STR);

    void Traverse(const IStructType* sType, ITabVisitor* visitor);

    std::string AbsolutePath(const std::string& path);
    bool SplitPath(const std::string& src,
        std::string* p = nullptr, std::string* f = nullptr, std::string* e = nullptr);
    bool IsDir(const std::string& path);
    bool IsFile(const std::string& file);
    bool CreateDir(const std::string& path);
    bool TraverseDir(const std::string& path, const std::function<bool(const std::string&, bool)> visitor);
    std::vector<std::string> CollectDir(const std::string& path,
        const std::string& suffix = EMPTY_STR, bool ignoreDir = true);
}

CFG_NAMESPACE_END
