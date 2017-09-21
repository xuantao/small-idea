#pragma once
#include "gcf/gcf.h"

GCF_NAMESPACE_BEGIN

namespace cs_util
{
    std::string ToString(const IValue* val);
    std::string TypeName(const IType* type);
    std::string TypeName(const IType* type, const IScope* pScope);
    std::string Value(const IVariate* var);
    std::string Value(const IVariate* var, const IScope* scope);
    std::string DefValue(const IType* type);

    void ReadVar(
        std::ostream& stream,
        IType* type,
        const std::string& reader,
        const std::string& valName,
        const std::string& rdName,
        int tab,
        bool useNs
    );

    void ReadVar(std::ostream& stream, IVariate* var, int tab, bool useNs);
    void ReadVarDetail(std::ostream& stream, IType* type, int tab, bool useNs);

    void WriteVar(
        std::ostream& stream,
        IType* type,
        const std::string& writer,
        const std::string& valName,
        const std::string& wrName,
        int tab,
        bool useNs
    );
    void WriteVar(std::ostream& stream, IVariate* var, int tab, bool useNs);
    void WriteVarDetail(std::ostream& stream, IType* type, int tab, bool useNs);
}

GCF_NAMESPACE_END
