﻿#pragma once

#include "gcf/gcf.h"
#include <ostream>

GCF_NAMESPACE_BEGIN

namespace cs
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
        void DeclRead(const IType* type);
        void DeclWrite(const IType* type);
        void Clear();

    protected:
        bool _isFirst = true;
        int _tab = 0;
        std::ostream* _stream;
    };
}

GCF_NAMESPACE_END