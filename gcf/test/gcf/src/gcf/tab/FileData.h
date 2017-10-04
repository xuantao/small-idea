#pragma once
#include <memory>

namespace tab
{
    class FileData : public std::enable_shared_from_this<FileData>
    {
    public:
        FileData();
        ~FileData();

        FileData(const FileData&) = delete;
        FileData& operator = (const FileData&) = delete;
    public:
        bool Load(const char* file, bool text = true);
        char* Data() const { return _data; }
        size_t Size() const { return _size; }

    protected:
        char* _data = nullptr;
        size_t _size = 0;
    };
}
