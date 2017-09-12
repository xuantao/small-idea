#pragma once

#include "gcf/gcf.h"
#include <ostream>

CFG_NAMESPACE_BEGIN

namespace cpp
{
    class Serialize
    {
    public:
        Serialize();
        ~Serialize();

    public:
        bool Begin(const IScope* global, std::string& path, std::string& name);
        bool OnType(const IType* type);
        void End();

    protected:
        std::ostream& DeclRead(std::ostream& stream, const IType* type, bool isDecl);
        std::ostream& DeclWrite(std::ostream& stream, const IType* type, bool isDecl);
        void ImplRead(const IType* type);
        void ImplWrite(const IType* type);
        void Clear();

    protected:
        bool _isFirst = true;
        int _tab = 0;
        std::ostream* _header;
        std::ostream* _cpp;
    };
}

CFG_NAMESPACE_END
