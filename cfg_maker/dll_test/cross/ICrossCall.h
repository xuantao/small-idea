/*
 * 交叉调用
 * 针对跨语言需要定义各种导出接口的情况
 * 使用固定数据序列化实现参数传递
*/
#pragma once

#include "ISerialize.h"

namespace cross_call
{
    class IContext
    {
    public:
        virtual ~IContext() {}
    public:
        virtual serialize::IReader* Param() = 0;
        virtual serialize::IWriter* Ret() = 0;
    };

    class IProcessor
    {
    public:
        virtual ~IProcessor() {}
    public:
        virtual void Process(IContext* context) = 0;
    };

    class IInvoker
    {
    public:
        virtual ~IInvoker() {}
    public:
        virtual serialize::IWriter* Begin(uint32_t module) = 0;
        virtual serialize::IReader* End() = 0;
    };

    class ICrossCall
    {
    public:
        virtual ~ICrossCall() {}
    public:
        virtual void DoCall() = 0;
    };
}
