#include "Utility.h"
#include "Interface.h"
#include "Type.h"
#include <algorithm>
#include <sstream>
#include <iostream>
#include <set>
#include <queue>
#include <Windows.h>

#undef min
#undef max
CFG_NAMESPACE_BEGIN

namespace utility
{
    //std::string STR_EMPTY;

    /*
     * 使用递归可以从头开始向数组插入元素
    */
    static void Absolute(const IType* type, std::vector<std::string>& path)
    {
        if (type->Belong() == nullptr)
            return; // top scope

        Absolute(type->Belong(), path);
        path.push_back(type->Name());
    }

    static IType* FindType(IType* scope, const std::vector<std::string>& path, int begin = 0, int end = -1)
    {
        if (end < 0)
            end = (int)path.size();
        else
            end = std::min(end, (int)path.size());

        // param check
        if (end - begin < 0)
            return nullptr;
        if (begin >= (int)path.size())
            return nullptr;

        /*
         * find first own type
        */
        while (scope)
        {
            IType* temp = scope;
            if (temp->TypeSet())
            {
                scope = temp->TypeSet()->Get(path[begin]);
                if (scope)
                    break;
            }

            scope = temp->Belong();
        }

        /*
         * get the last own type
        */
        for (int i = begin + 1; i < end; ++i)
        {
            if (scope == nullptr)
                break;
            else if (scope->TypeSet() == nullptr)
                scope = nullptr;
            else
                scope = scope->TypeSet()->Get(path[i]);
        }
        return scope;
    }

    static bool sUniqueName(std::vector<UniqueVarName>& outNames, const IStructType* sType, const std::string& owner)
    {
        std::set<std::string> allVars;
        std::set<std::string> conficts;
        const IVarSet* varSet = sType->VarSet();

        /* get name conflict */
        for (int i = 0; i < varSet->Size(); ++i)
        {
            const IVariate* var = varSet->Get(i);
            if (var->IsConst())
                continue;

            if (allVars.find(var->Name()) != allVars.end())
                conficts.insert(var->Name());
            else
                allVars.insert(var->Name());
        }

        for (int i = 0; i < varSet->Size(); ++i)
        {

            const IVariate* var = varSet->Get(i);
            if (var->IsConst())
                continue;

            TypeCategory typeCategory = var->Type()->TypeCat();
            std::string uniqName;
            if (conficts.find(var->Name()) != conficts.end())
            {
                if (owner.empty())
                    uniqName = var->Belong()->Name() + "::" + var->Name();
                else
                    uniqName = owner + "." + var->Belong()->Name() + "::" + var->Name();
            }
            else
            {
                if (owner.empty())
                    uniqName = var->Name();
                else
                    uniqName = owner + "." + var->Name();
            }

            if (var->Type()->TypeCat() == TypeCategory::Struct)
                sUniqueName(outNames, static_cast<const StructType*>(var->Type()), uniqName);
            else
                outNames.push_back(UniqueVarName(var, uniqName));
        }

        return true;
    }

    static bool TabTraverse(const IStructType* aType, ITabVisitor* visitor, const std::string& title, const std::string& path);
    static bool TabTraverse(const IVariate* var, const IArrayType* aType, ITabVisitor* visitor, const std::string& title, const std::string& path)
    {
        IType* original = aType->Original();
        if (aType->Original() != aType->Prev())
        {
            ERROR_NOT_ALLOW;    // 多维数组
            return true;
        }

        if (original->TypeCat() == TypeCategory::Struct && aType->Length() <= 0)
        {
            ERROR_NOT_ALLOW;    // 结构体不支持变长数组
            return true;
        }

        if (original->TypeCat() == TypeCategory::Raw)
        {
            if (!visitor->OnVar(var, static_cast<const IRawType*>(original), title, path, aType->Length()))
                return false;
        }
        else if (original->TypeCat() == TypeCategory::Enum)
        {
            if (!visitor->OnVar(var, static_cast<const IEnumType*>(original), title, path, aType->Length()))
                return false;
        }
        else if (original->TypeCat() == TypeCategory::Struct)
        {
            for (int i = 0; i < aType->Length(); ++i)
            {
                std::string arPath = path + "[" + std::to_string(i) + "]";
                std::string arTitle = title + "_" + std::to_string(i + 1);
                if (!TabTraverse(static_cast<const IStructType*>(original), visitor, title, arPath))
                    return false;
            }
        }
        else
        {
            ERROR_NOT_ALLOW;
        }

        return true;
    }

    static bool TabTraverse(const IStructType* sType, ITabVisitor* visitor, const std::string& title, const std::string& path)
    {
        const IVarSet* vars = sType->VarSet();
        for (int i = 0; i < vars->Size(); ++i)
        {
            const IVariate* var = vars->Get(i);
            if (var->IsConst())
                continue;

            const IType* type = var->Type();
            std::string varPath;
            std::string varTitle;
            if (path.empty())
                varPath = var->Name();
            else
                varPath = path + '.' + var->Name();

            if (title.empty())
                varTitle = var->Name();
            else
                varTitle = title + '.' + var->Name();

            if (type->TypeCat() == TypeCategory::Raw)
            {
                if (!visitor->OnVar(var, static_cast<const IRawType*>(type), varTitle, varPath))
                    return false;
            }
            else if (type->TypeCat() == TypeCategory::Enum)
            {
                if (!visitor->OnVar(var, static_cast<const IEnumType*>(type), varTitle, varPath))
                    return false;
            }
            else if (type->TypeCat() == TypeCategory::Array)
            {
                if (!TabTraverse(var, static_cast<const IArrayType*>(type), visitor, varTitle, varPath))
                    return false;
            }
            else if (type->TypeCat() == TypeCategory::Struct)
            {
                if (!TabTraverse(static_cast<const IStructType*>(type), visitor, varTitle, varPath))
                    return false;
            }
            else
            {
                ERROR_NOT_ALLOW;
            }
        }
        return true;
    }

    bool Convert(const std::string& str, bool& out)
    {
        return Convert(str, out, out);
    }

    bool Convert(const std::string& str, bool& out, bool def)
    {
        out = def;
        if (str == "true")
            out = true;
        else if (str == "false")
            out =false;
        else
            return false;
        return true;
    }

    bool Convert(const std::string& str, int& out)
    {
        return Convert(str, out, out);
    }

    bool Convert(const std::string& str, int& out, int def)
    {
        out = def;
        try
        {
            out = std::stoi(str);
        }
        catch (std::exception e)
        {
            return false;
        }
        return true;
    }

    bool Convert(const std::string& str, float& out)
    {
        return Convert(str, out, out);
    }

    bool Convert(const std::string& str, float& out, float def)
    {
        out = def;
        try
        {
            out = std::stof(str);
        }
        catch (std::exception e)
        {
            return false;
        }
        return true;
    }

    std::string TrimLeft(const std::string& str, const std::string& trim)
    {
        if (str.length() == 0)
            return str;

        std::string::size_type beg = 0;
        for (; beg < str.length(); ++beg)
        {
            if (std::string::npos == trim.find(str[beg]))
                break;
        }

        return str.substr(beg);
    }

    std::string TrimRight(const std::string& str, const std::string& trim)
    {
        if (str.length() == 0)
            return str;

        std::string::size_type end = str.length();
        while (end > 0 && std::string::npos != trim.find(str[end - 1]))
        {
            --end;
        }

        return str.substr(0, end);
    }

    std::string Trim(const std::string& str, const std::string& trim)
    {
        if (str.length() == 0)
            return str;

        return TrimLeft(TrimRight(str, trim), trim);

        //std::string::size_type beg = 0;
        //std::string::size_type end = str.length() - 1;
        //for (; beg < str.length(); ++beg)
        //{
        //    if (std::string::npos == trim.find(str[beg]))
        //        break;
        //}

        //for (; end > beg; --end)
        //{
        //    if (std::string::npos == trim.find(str[end]))
        //        break;
        //}

        //return str.substr(beg, end + 1 - beg);
    }

    std::string Replace(const std::string& str, const std::string& _r, const std::string& _n)
    {
        std::string temp = str;
        std::string::size_type pos = temp.find(_r);
        while (pos != std::string::npos)
        {
            temp = temp.replace(pos, _r.length(), _n);
            pos = temp.find(_r, pos + _n.length());
        }

        return temp;
    }

    std::string TrimFileSuffix(const std::string& file, char c/* = '.'*/)
    {
        std::string::size_type pos = file.find_last_of(c);
        if (pos == std::string::npos)
            return file;

        std::string::size_type s_l = file.find_last_of("/");
        std::string::size_type s_r = file.find_last_of("\\");
        if (s_l == std::string::npos)
            s_l = 0;
        if (s_r == std::string::npos)
            s_r = 0;

        if (pos > std::max(s_l, s_r))
            return file.substr(0, pos - 1);
        else
            return file;
    }

    std::string Contact(const std::vector<std::string>& path, const std::string& c)
    {
        if (path.empty())
            return EMPTY_STR;

        std::stringstream stream;
        stream << path.front();
        for (size_t i = 1; i < path.size(); ++i)
            stream << c << path[i];

        return stream.str();
    }

    std::vector<std::string> Split(const std::string& str, const std::string& s)
    {
        std::vector<std::string> ret;
        std::string::size_type beg = 0;
        std::string::size_type pos = str.find_first_of(s);

        while (std::string::npos != pos)
        {
            ret.push_back(str.substr(beg, pos - beg));
            beg = pos + s.length();
            pos = str.find_first_of(s, beg);
        }

        if (beg < str.length())
            ret.push_back(str.substr(beg));

        return std::move(ret);
    }

    std::vector<std::string> Absolute(const IType* type)
    {
        if (type == nullptr)
            return EMPTY_VEC_STR;
        if (type->TypeCat() == TypeCategory::Raw)
            return EMPTY_VEC_STR;

        std::vector<std::string> path;
        Absolute(type, path);

        return std::move(path);
    }

    std::vector<std::string> Relative(const IType* self, const IType* other)
    {
        if (self == nullptr || other == nullptr)
            return EMPTY_VEC_STR;
        if (self->TypeCat() == TypeCategory::Raw || other->TypeCat() == TypeCategory::Raw)
            return EMPTY_VEC_STR;

        const std::vector<std::string> sp = Absolute(self);
        const std::vector<std::string> op = Absolute(other);

        size_t beg = 0;
        std::vector<std::string> path;
        for (; beg < std::min(sp.size(), op.size()); ++beg)
        {
            if (sp[beg] != op[beg])
                break;
        }

        return std::vector<std::string>(op.begin() + beg, op.end());
    }

    IType* FindType(IType* scope, const std::string& path)
    {
        if (scope == nullptr)
            return nullptr;
        if (scope->Name() == path)
            return scope;
        return FindType(scope, utility::Split(path, "."));
    }

    IVariate* FindVar(IType* scope, const std::string& path)
    {
        IVariate* var = nullptr;
        const IType* type = scope;
        std::vector<std::string> vec = utility::Split(path, ".");

        if (vec.size() > 1)
            type = FindType(scope, vec, 0, (int)vec.size() - 1);

        if (type == nullptr)
            return nullptr;

        while (type && !var)
        {
            if (type->VarSet())
                var = type->VarSet()->Get(vec.back());
            type = type->Belong();
        }
        return var;
    }

    std::ostream& Tab(std::ostream& stream, int tab)
    {
        if (tab <= 0)
            return stream;

        return stream << std::string(tab * 4, ' ');
    }

    std::vector<UniqueVarName> UniqueName(const IStructType* sType, const std::string& owner/* = EMPTY_STR*/)
    {
        std::vector<UniqueVarName> varNames;
        if (sType = nullptr)
            return std::move(varNames);

        sUniqueName(varNames, sType, owner);
        return std::move(varNames);
    }

    void Traverse(const IStructType* sType, ITabVisitor* visitor)
    {
        if (!visitor->OnStart(sType))
            return;
        TabTraverse(sType, visitor, "", "");
        visitor->OnEnd();
    }

    std::string AbsolutePath(const std::string& path)
    {
        std::vector<std::string> ps = Split(Replace(path, "\\", "/"), "/");
        int idx = 0;

        while (idx < ps.size())
        {
            if (ps.empty() || ps[idx] == ".")
            {
                ps.erase(ps.begin() + idx);
            }
            else if (ps[idx] == ".." && idx && ps[idx - 1] != "..")
            {
                ps.erase(ps.begin() + idx);
                --idx;
                ps.erase(ps.begin() + idx);
            }
            else
            {
                ++idx;
            }
        }

        return Contact(ps, "/");
    }

    bool SplitPath(const std::string& src, std::string* p/* = nullptr*/, std::string* f/* = nullptr*/, std::string* e/* = nullptr*/)
    {
        std::string::size_type dot = src.find_last_of('.');
        std::string::size_type slash = src.find_last_of('/');
        std::string::size_type slash2 = src.find_last_of('\\');

        if (slash == std::string::npos)
            slash = slash2;
        else if (slash2 != std::string::npos)
            slash = std::max(slash, slash2);

        if (dot != std::string::npos)
        {
            if (slash != std::string::npos)
            {
                if (dot > slash)
                {
                    if (p) *p = src.substr(0, slash);
                    if (f) *f = src.substr(slash + 1, dot - slash - 1);
                    if (e) *e = src.substr(dot + 1);
                }
                else
                {
                    if (p) *p = src.substr(0, slash);
                    if (f) *f = src.substr(slash + 1);
                    if (e) *e = EMPTY_STR;
                }
            }
            else
            {
                if (p) *p = EMPTY_STR;
                if (f) *f = src.substr(0, dot);
                if (e) *e = src.substr(dot + 1);
            }
        }
        else if (slash != std::string::npos)
        {
            if (p) *p = src.substr(0, slash);
            if (f) *f = src.substr(slash + 1);
            if (e) *e = EMPTY_STR;
        }
        else
        {
            if (p) *p = EMPTY_STR;
            if (f) *f = src;
            if (e) *e = EMPTY_STR;
        }

        return true;
    }

    bool IsDir(const std::string& path)
    {
        uint32_t mask = ::GetFileAttributesA(path.c_str());
        if (mask == INVALID_FILE_ATTRIBUTES)
            return false;
        return mask & FILE_ATTRIBUTE_DIRECTORY;
    }

    bool IsFile(const std::string& file)
    {
        uint32_t mask = ::GetFileAttributesA(file.c_str());
        if (mask == INVALID_FILE_ATTRIBUTES)
            return false;
        return !(mask & FILE_ATTRIBUTE_DIRECTORY);
    }

    bool CreateDir(const std::string& path)
    {
        std::string p;
        std::vector<std::string> sp = Split(Replace(path, "\\", "/"), "/");

        for (size_t i = 0; i < sp.size(); ++i)
        {
            if (sp[i].empty() || sp[i] == ".")
                continue;

            if (p.empty())
                p = sp[i];
            else
                p += '/' + sp[i];

            if (sp[i] == "..")
                continue;

            if (!IsDir(p) && !::CreateDirectoryA(p.c_str(), nullptr))
            {
                std::cerr << "create directory " << p << " failed" << std::endl;
                return false;
            }
        }

        return true;
    }

    bool TraverseDir(const std::string& path, const std::function<bool(const std::string &, bool)> visitor)
    {
        if (!IsDir(path))
            return false;

        std::queue<std::string> dirs;
        bool success = true;

        dirs.push(TrimRight(path, "/\\"));
        while (success && !dirs.empty())
        {
            std::string p = dirs.front();
            dirs.pop();

            WIN32_FIND_DATAA ffd;
            HANDLE hFind = ::FindFirstFileA((p + "/*").c_str(), &ffd);
            if (hFind == INVALID_HANDLE_VALUE)
                return false;

            while (success && ::FindNextFileA(hFind, &ffd) != 0)
            {
                std::string f = p + "/" + ffd.cFileName;
                if (ffd.cFileName[0] == '.' && ffd.cFileName[1] == 0)
                    continue;
                if (ffd.cFileName[0] == '.' && ffd.cFileName[1] == '.')
                    continue;

                if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    dirs.push(f);

                success = visitor(f, ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
            }

            ::FindClose(hFind);
        }

        return success;
    }

    std::vector<std::string> CollectDir(const std::string& path,
        const std::string& suffix/* = EMPTY_STR*/, bool ignoreDir/* = true*/)
    {
        std::vector<std::string> files;
        utility::TraverseDir(path, [&](const std::string& file, bool dir) {
            if (dir && ignoreDir)
                return true;

            // 过滤后缀
            if (
                !suffix.empty() &&
                (
                    file.length() <= suffix.length() ||
                    suffix != file.substr(file.length() - suffix.length())
                    )
                )
            {
                return true;
            }

            files.push_back(file);
            return true;
        });

        return std::move(files);
    }
}

CFG_NAMESPACE_END
