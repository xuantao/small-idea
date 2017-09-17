#pragma once
#include <ostream>
#include <functional>
#include "../gcf/gcf.h"
#include "TupleLogger.h"

GCF_NAMESPACE_BEGIN

namespace utility
{
    static const std::string EMPTY_STR;
    static const std::vector<std::string> EMPTY_VEC_STR;

    /*
     * common way to setup tab file title
    */
    class ITabVisitor
    {
    public:
        virtual ~ITabVisitor() {}
    public:
        virtual bool OnVar(const IVariate* var, const IType* rType,
            const std::string& title) = 0;
    };

    uint32_t HashValue(const char* str);
    uint32_t HashValue(const IType* ty);
    uint32_t HashValue(const ICrossCall* cross);

    bool Convert(const std::string& str, bool& out);
    bool Convert(const std::string& str, int& out);
    bool Convert(const std::string& str, float& out);

    std::string TrimFileSuffix(const std::string& file, char c = '.');
    std::string TrimLeft(const std::string& str, const std::string& trim);
    std::string TrimRight(const std::string& str, const std::string& trim);
    std::string Trim(const std::string& str, const std::string& trim);
    std::string Replace(const std::string& str, const std::string& _r, const std::string& _n);

    std::string Contact(const std::vector<std::string>& path, const std::string& c);
    std::vector<std::string> Split(const std::string& str, const std::string& s);

    std::vector<std::string> Absolute(const IScope* scope);
    std::vector<std::string> Absolute(const IType* type);
    std::vector<std::string> Relative(const IType* self, const IScope* scope);
    std::vector<std::string> Relative(const IScope* self, const IScope* scope);

    /*
     * 查找类型
    */
    IType* FindType(IScope* scope, const std::string& path);

    /*
     * 查找变量
    */
    IVariate* FindVar(IScope* scope, const std::string& path);

    std::ostream& Tab(std::ostream& stream, int tab);

    void Traverse(const IStructType* sType, ITabVisitor* visitor);

    std::string AbsolutePath(const std::string& path);
    std::string ContactPath(const std::string& l, const std::string& r);
    bool SplitPath(const std::string& src,
        std::string* p = nullptr, std::string* f = nullptr, std::string* e = nullptr);
    bool IsDir(const std::string& path);
    bool IsFile(const std::string& file);
    bool CreateDir(const std::string& path);
    bool TraverseDir(const std::string& path, const std::function<bool(const std::string&, bool)> visitor);
    std::vector<std::string> CollectDir(const std::string& path,
        const std::string& suffix = EMPTY_STR, bool ignoreDir = true);

    // log
    template <class... Types>
    void Log(std::ostream& out, const char* format, Types&&... args)
    {
        typedef detail::TupleLogger<sizeof...(Types)> Logger;
        std::tuple<Types...> tp(args...);
        std::regex reg("\\{\\d+\\}");
        std::cmatch cm;
        std::ptrdiff_t pos = 0;

        while (std::regex_search(&format[pos], cm, reg))
        {
            int idx = 0;
            std::cout.write(format + pos, cm.position());
            pos += cm.position();
            idx = atoi(format + pos + 1);

            if (idx >= Logger::C)
            {
                out << "(out range at pos:" << pos << ", ";
                out.write(format + pos, cm.length());
                out << ")";
            }
            else
            {
                Logger::log(out, idx, tp);
            }

            pos += cm.length();
        }

        out << format + pos << std::endl;
    }
}

GCF_NAMESPACE_END
