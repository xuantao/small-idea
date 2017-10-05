/*
 * 交叉调用
 * 针对跨语言需要定义各种导出接口的情况
 * 使用固定数据序列化实现参数传递
*/
#pragma once
#include "../serialize/ISerialize.h"
#include <memory>

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
        virtual int ID() const = 0;
        virtual void Process(IContext* context) = 0;
    };

    typedef std::shared_ptr<IProcessor> ProcessorPtr;

    class IInvoker
    {
    public:
        virtual ~IInvoker() {}
    public:
        virtual serialize::IWriter* Begin(int32_t module) = 0;
        virtual serialize::IReader* End() = 0;
    };
}
