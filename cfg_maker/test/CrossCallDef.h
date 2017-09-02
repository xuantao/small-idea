/*
 * 交叉调用
 * 针对跨语言需要定义各种导出接口的情况
 * 使用固定数据序列化实现参数传递
*/
#pragma once

#include "Serialize.h"

namespace cross_call
{
    class IProcessor
    {
    public:
        virtual ~IProcessor() {}

    public:
        virtual uint32_t GetModuleID() const = 0;
        virtual uint32_t GetHashCode() const = 0;
        virtual void Process(serialize::IReader* reader, serialize::IWriter* writer) = 0;
    };

    class ICrossCaller
    {
    public:
        virtual ~ICrossCaller() {}

    public:
        virtual serialize::IWriter* BeginCall(uint32_t module, uint32_t code) = 0;
        virtual serialize::IReader* EndCall() = 0;
    };
}
