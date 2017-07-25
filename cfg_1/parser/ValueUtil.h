﻿#pragma once
#include "CfgDef.h"

CFG_NAMESPACE_BEGIN

namespace value_util
{
    bool IsRaw(const IValue* val, RawCategory raw, bool ref = true);

    bool Value(const IValue* val, bool& b);
    bool Value(const IValue* val, int& i);
    bool Value(const IValue* val, float& f);
    bool Value(const IValue* val, std::string& str);

    /*
     * 创建原生值
    */
    IValue* Create(RawCategory raw, const std::string& value);

    /*
     * 创建引用值
    */
    IValue* Create(const IVariate* var);
}

CFG_NAMESPACE_END
