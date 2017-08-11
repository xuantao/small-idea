#pragma once
#include "Interface.h"

CFG_NAMESPACE_BEGIN

namespace value_util
{
    bool AsRaw(RawCategory raw, const IValue* val);

    bool Value(const IValue* val, bool& b);
    bool Value(const IValue* val, int& i);
    bool Value(const IValue* val, float& f);
    bool Value(const IValue* val, std::string& str);

    std::string ToString(const IValue* val);
    std::string DefValue(RawCategory raw);

    /*
     * 创建默认值
    */
    IValue* Create(RawCategory raw);

    /*
     * 创建原生值
    */
    IValue* Create(RawCategory raw, const std::string& value);

    /*
     * 创建引用值
    */
    IValue* Create(IVariate* var);
}

CFG_NAMESPACE_END
