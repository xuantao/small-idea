﻿#include "Utility.h"
#include <algorithm>
#include <sstream>

CFG_NAMESPACE_BEGIN

namespace util
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

    static const IType* FindType(const IType* scope, const std::vector<std::string>& path, int begin = 0, int end = -1)
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
            const IType* temp = scope;
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

    std::vector<std::string> Split(const std::string& str, char s)
    {
        std::vector<std::string> ret;
        std::string::size_type beg = 0;
        std::string::size_type pos = str.find_first_of(s);

        while (std::string::npos != pos)
        {
            ret.push_back(str.substr(beg, pos - beg));
            beg = pos + 1;
            pos = str.find_first_of(s, beg);
        }
        ret.push_back(str.substr(beg));

        return std::move(ret);
    }

    std::vector<std::string> Absolute(const IType* type)
    {
        if (type == nullptr)
            return EMPTY_VEC_STR;
        if (type->Category() == TypeCategory::Raw)
            return EMPTY_VEC_STR;

        std::vector<std::string> path;
        Absolute(type, path);

        return std::move(path);
    }

    std::vector<std::string> Relative(const IType* self, const IType* other)
    {
        if (self == nullptr || other == nullptr)
            return EMPTY_VEC_STR;
        if (self->Category() == TypeCategory::Raw || other->Category() == TypeCategory::Raw)
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

    const IType* FindType(const IType* scope, const std::string& path)
    {
        if (scope == nullptr)
            return nullptr;
        if (scope->Name() == path)
            return scope;
        return FindType(scope, util::Split(path, '.'));
    }

    const IVariate* FindVar(const IType* scope, const std::string& path)
    {
        const IVariate* var = nullptr;
        const IType* type = scope;
        std::vector<std::string> vec = util::Split(path, '.');

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
}

CFG_NAMESPACE_END
