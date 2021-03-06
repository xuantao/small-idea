﻿#include "SerText.h"
#include <cassert>

namespace serialize
{
    std::vector<char*> sSplit(char* text)
    {
        std::vector<char*> ret;
        ret.push_back(text);

        while (*text)
        {
            if (*text == ',')
            {
                *text = 0;
                ++text;

                ret.push_back(text);
            }
            else
            {
                ++text;
            }
        }

        return std::move(ret);
    }

    //////////////////////////////////////////////////////////////////////////
    // TextReader
    TextReader::TextReader(ITokenReadStream* stream) : _stream(stream)
    {
    }

    TextReader::~TextReader()
    {
    }

    bool TextReader::ArrayBegin(int& length, const char* name/*= nullptr*/)
    {
        assert(_isArray == false && "struct serialize text only support one array");
        if (_isArray)
            return false;

        const char* data = _stream->Read();
        if (data == nullptr)
            return false;

        size_t len = std::strlen(data);
        if (_buff.size() <= len)
            _buff.resize(len + 1, 0);
        else
            _buff[len] = 0;

        memcpy(&_buff[0], data, len);

        _isArray = true;
        _idx = 0;
        _array = sSplit(&_buff[0]);
        length = (int)_array.size();
        return true;
    }

    bool TextReader::ArrayEnd()
    {
        _isArray = false;
        _idx = 0;
        _array.clear();
        return true;
    }

    bool TextReader::Read(bool& val, const char* name/*= nullptr*/)
    {
        const char* token = DoRead();
        if (token == nullptr)
            return false;
        if (*token == 0)
            return true;

        if (
            std::strcmp(token, "true") == 0 ||
            std::strcmp(token, "True") == 0 ||
            std::strcmp(token, "TRUE") == 0 ||
            std::strcmp(token, "1") == 0
            )
        {
            val = true;
        }
        else
        {
            val = false;
        }

        return true;
    }

    bool TextReader::Read(int8_t& val, const char* name/*= nullptr*/)
    {
        const char* token = DoRead();
        if (token == nullptr) return false;
        if (*token == 0) return true;

        val = (int8_t)std::atoi(token);
        return true;
    }

    bool TextReader::Read(int32_t& val, const char* name/*= nullptr*/)
    {
        const char* token = DoRead();
        if (token == nullptr) return false;
        if (*token == 0) return true;

        val = (int32_t)std::atoi(token);
        return true;
    }

    bool TextReader::Read(int64_t& val, const char* name/*= nullptr*/)
    {
        const char* token = DoRead();
        if (token == nullptr) return false;
        if (*token == 0) return true;

        val = (int64_t)std::atoll(token);
        return true;
    }

    bool TextReader::Read(float& val, const char* name/*= nullptr*/)
    {
        const char* token = DoRead();
        if (token == nullptr) return false;
        if (*token == 0) return true;

        val = (float)std::atof(token);
        return true;
    }

    bool TextReader::Read(double& val, const char* name/*= nullptr*/)
    {
        const char* token = DoRead();
        if (token == nullptr) return false;
        if (*token == 0) return true;

        val = std::atof(token);
        return true;
    }

    bool TextReader::Read(std::string& val, const char* name/*= nullptr*/)
    {
        const char* token = DoRead();
        if (token == nullptr) return false;
        if (*token == 0) return true;

        val = token;
        return true;
    }

    const char* TextReader::DoRead()
    {
        if (_isArray)
        {
            assert(_idx <= (int)_array.size());
            return _array[_idx++];
        }
        else
        {
            return _stream->Read();
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // TextWriter
    TextWriter::TextWriter(ITokenWriteStream* stream) : _stream(stream)
    {
    }

    TextWriter::~TextWriter()
    {
    }

    bool TextWriter::ArrayBegin(int length, const char* name/*= nullptr*/)
    {
        assert(_isArray == false && "struct serialize text only support one array");
        if (_isArray)
            return false;

        //assert(_array.tellp() == 0 && "last array has some error");

        _isArray = true;
        return true;
    }

    bool TextWriter::ArrayEnd()
    {
        std::string str = _array.str();
        _isArray = false;
        _array.str("");
        _array.clear();

        return _stream->Write(str.c_str());
    }

    bool TextWriter::Write(bool val, const char* name/*= nullptr*/)
    {
        return DoWrite(val ? "true" : "false");
    }

    bool TextWriter::Write(int8_t val, const char* name/*= nullptr*/)
    {
        std::string str = std::to_string(val);
        return DoWrite(str.c_str());
    }

    bool TextWriter::Write(int32_t val, const char* name/*= nullptr*/)
    {
        std::string str = std::to_string(val);
        return DoWrite(str.c_str());
    }

    bool TextWriter::Write(int64_t val, const char* name/*= nullptr*/)
    {
        std::string str = std::to_string(val);
        return DoWrite(str.c_str());
    }

    bool TextWriter::Write(float val, const char* name/*= nullptr*/)
    {
        std::string str = std::to_string(val);
        return DoWrite(str.c_str());
    }

    bool TextWriter::Write(double val, const char* name/*= nullptr*/)
    {
        std::string str = std::to_string(val);
        return DoWrite(str.c_str());
    }

    bool TextWriter::Write(const std::string& val, const char* name/*= nullptr*/)
    {
        return DoWrite(val.c_str());
    }

    bool TextWriter::DoWrite(const char* str)
    {
        if (_isArray)
        {
            if (_array.tellp()) _array << ",";
            _array << str;
            return true;
        }
        else
        {
            return _stream->Write(str);
        }
    }
}
