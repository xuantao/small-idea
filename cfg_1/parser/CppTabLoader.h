#pragma once
#include "Interface.h"
#include <ostream>

CFG_NAMESPACE_BEGIN

namespace detail
{
    class CppTabLoader : public IVarVistor
    {
    public:
        CppTabLoader(std::ostream* stream, int tab) : _stream(stream), _tab(tab)
        { }

    public:
        static bool Declare(IStructType* sType, std::ostream& stream);

    public:
        virtual bool OnStart(const IStructType* sType);
        virtual bool OnEnd();
        virtual bool OnVar(const IVariate* var, const IRawType* rType,
            const std::string& title, const std::string& path);
        virtual bool OnVar(const IVariate* var, const IEnumType* eType,
            const std::string& title, const std::string& path);
        virtual bool OnVar(const IVariate* var, const IRawType* rType,
            const std::string& title, const std::string& path, int length);
        virtual bool OnVar(const IVariate* var, const IEnumType* eType,
            const std::string& title, const std::string& path, int length);

    protected:
        bool _newLine;
        std::ostream* _stream;
        int _tab;
    };
}

CFG_NAMESPACE_END
