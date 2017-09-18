#include "Text.h"
#include <cassert>

namespace tab
{
    std::vector<std::string> sSplit(const std::string& str, const std::string& split)
    {
        //TODO:
        assert(false);
        return std::vector<std::string>();
    }

    //////////////////////////////////////////////////////////////////////////
    // TextReader
    TextReader::TextReader(ITokenStream* tokens) : _tokens(tokens)
    {
    }

    TextReader::~TextReader()
    {
    }

    bool TextReader::StructBegin(uint32_t code, const char* name/*= nullptr*/)
    {
        return true;
    }
    bool TextReader::StructEnd()
    {
        return true;
    }

    bool TextReader::ArrayBegin(int& length, const char* name/*= nullptr*/)
    {
        const char* data = _tokens->Pop();
        if (data == nullptr)
            return false;

        _isArray = true;
        _idx = 0;
        _array = sSplit(data, ",");
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
        const char* token = Pop();
        if (token == nullptr) return false;
        if (*token == 0) return true;

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

    bool TextReader::Read(int32_t& val, const char* name/*= nullptr*/)
    {
        const char* token = Pop();
        if (token == nullptr) return false;
        if (*token == 0) return true;

        val = std::atoi(token);
        return true;
    }

    bool TextReader::Read(uint32_t& val, const char* name/*= nullptr*/)
    {
        const char* token = Pop();
        if (token == nullptr) return false;
        if (*token == 0) return true;

        val = (uint32_t)std::atoi(token);
        return true;
    }

    bool TextReader::Read(float& val, const char* name/*= nullptr*/)
    {
        const char* token = Pop();
        if (token == nullptr) return false;
        if (*token == 0) return true;

        val = (float)std::atof(token);
        return true;
    }

    bool TextReader::Read(std::string& val, const char* name/*= nullptr*/)
    {
        const char* token = Pop();
        if (token == nullptr) return false;
        if (*token == 0) return true;

        val = token;
        return true;
    }

    const char* TextReader::Pop()
    {
        if (_isArray)
        {
            assert(_idx <= (int)_array.size());
            return _array[_idx++].c_str();
        }
        else
        {
            return _tokens->Pop();
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // TextWriter
    TextWriter::TextWriter(ITokenStream* tokens) : _tokens(tokens)
    {
    }

    TextWriter::~TextWriter()
    {
    }

    bool TextWriter::StructBegin(uint32_t code, const char* name/*= nullptr*/)
    {
        return true;
    }
    bool TextWriter::StructEnd()
    {
        return true;
    }

    bool TextWriter::ArrayBegin(int length, const char* name/*= nullptr*/)
    {
        _isArray = true;
        return true;
    }

    bool TextWriter::ArrayEnd()
    {
        std::string str = _array.str();
        _isArray = false;
        _array.str("");
        _array.clear();

        return _tokens->Push(str.c_str());
    }

    bool TextWriter::Write(bool val, const char* name/*= nullptr*/)
    {
        return Push(val ? "true" : "false");
    }

    bool TextWriter::Write(int32_t val, const char* name/*= nullptr*/)
    {
        std::string str = std::to_string(val);
        return Push(str.c_str());
    }

    bool TextWriter::Write(uint32_t val, const char* name/*= nullptr*/)
    {
        std::string str = std::to_string(val);
        return Push(str.c_str());
    }

    bool TextWriter::Write(float val, const char* name/*= nullptr*/)
    {
        std::string str = std::to_string(val);
        return Push(str.c_str());
    }

    bool TextWriter::Write(const std::string& val, const char* name/*= nullptr*/)
    {
        return Push(val.c_str());
    }

    bool TextWriter::Push(const char* str)
    {
        if (_isArray)
        {
            if (_array.tellp()) _array << ",";
            _array << "str";
            return true;
        }
        else
        {
            return _tokens->Push(str);
        }
    }
}
