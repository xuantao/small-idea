#include "FileData.h"
#include <fstream>
#include <cassert>

namespace tab
{
    FileData::FileData()
    {
    }

    FileData::~FileData()
    {
        delete _data;
    }

    bool FileData::Load(const char* file, bool text /* = true */)
    {
        assert(_data == nullptr);

        std::ifstream stream;
        if (text)
            stream.open(file);
        else
            stream.open(file, std::ios::in | std::ios::binary);

        if (!stream.is_open())
            return false;

        stream.seekg(0, std::ios::end);
        _size = stream.tellg();
        stream.seekg(0, std::ios::beg);

        // skip bom header
        if (text && _size >= 3)
        {
            int8_t bom[3] = {0};
            stream.read((char*)bom, 3);
            if (bom[0] == 0xef && bom[1] == 0xbb && bom[2] == 0xbf)
                _size -= 3;
            else
                stream.seekg(0, std::ios::beg);
        }

        _data = new char[_size + 1];
        _data[_size] = 0;

        stream.read(_data, _size);
        stream.close();
        return true;
    }
}
