/*
 * 序列化文本
 * 对数组元素使用','分割的，由此需要注意
 * 1. 不支持多维数组，无法解析数据
 * 2. 不支持自定义数据结构的数组
 * 3. 如果是字符串数组，单个元素中不能存在','
*/
#pragma once
#include "ISerialize.h"
#include <sstream>
#include <vector>

namespace serialize
{
    class TextReader : public IReader
    {
    public:
        TextReader(ITokenReadStream* stream);
        ~TextReader();

    public:
        virtual bool StructBegin(int32_t code, const char* name = nullptr) { return true; }
        virtual bool StructEnd() { return true; }

        virtual bool ArrayBegin(int& length, const char* name = nullptr);
        virtual bool ArrayEnd();
        virtual bool Read(bool& val, const char* name = nullptr);
        virtual bool Read(int8_t& val, const char* name = nullptr);
        virtual bool Read(int32_t& val, const char* name = nullptr);
        virtual bool Read(int64_t& val, const char* name = nullptr);
        virtual bool Read(float& val, const char* name = nullptr);
        virtual bool Read(double& val, const char* name = nullptr);
        virtual bool Read(std::string& val, const char* name = nullptr);
    protected:
        const char* DoRead();

    protected:
        ITokenReadStream* _stream = nullptr;
        bool _isArray = false;
        int _idx = 0;
        std::vector<char> _buff;
        std::vector<char*> _array;
    };

    class TextWriter : public IWriter
    {
    public:
        TextWriter(ITokenWriteStream* stream);
        ~TextWriter();

    public:
        virtual bool StructBegin(int32_t code, const char* name = nullptr) { return true; }
        virtual bool StructEnd() { return true; }

        virtual bool ArrayBegin(int length, const char* name = nullptr);
        virtual bool ArrayEnd();
        virtual bool Write(bool val, const char* name = nullptr);
        virtual bool Write(int8_t val, const char* name = nullptr);
        virtual bool Write(int32_t val, const char* name = nullptr);
        virtual bool Write(int64_t val, const char* name = nullptr);
        virtual bool Write(float val, const char* name = nullptr);
        virtual bool Write(double val, const char* name = nullptr);
        virtual bool Write(const std::string& val, const char* name = nullptr);

    protected:
        bool DoWrite(const char* str);

    protected:
        ITokenWriteStream* _stream = nullptr;
        bool _isArray = false;
        std::stringstream _array;
    };
}
