#pragma once

namespace tab
{
    class ITokenStream
    {
    public:
        virtual ~ITokenStream() {}
    public:
        virtual const char* Pop() = 0;
        virtual bool Push(const char* token) = 0;
    };
}
