#pragma once
#include "gcf/gcf.h"
#include <cassert>

GCF_NAMESPACE_BEGIN

namespace value_util
{
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
    IValue* Create(RawCategory raw, IVariate* var);
}

GCF_NAMESPACE_END
