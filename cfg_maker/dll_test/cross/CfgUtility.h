/*
 * 基础工具
*/
#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <stdio.h>

#define CFG_INVALID_ENUM    -1000000

namespace cfg
{
    namespace utility
    {
        typedef void(*LogCallback)(const char*);

        bool Convert(const std::string& str, bool& out);
        bool Convert(const std::string& str, int& out);
        bool Convert(const std::string& str, float& out);
        std::vector<std::string> Split(const std::string& str, const std::string& s);

        void SetLogger(LogCallback fn);
        LogCallback GetLogger();

        void Log(const char* fmt, ...);
    }

    /*
     * 读取文件
    */
    class TabFileData
    {
    public:
        TabFileData() : _buff(nullptr), _size(0)
        {
        }

        ~TabFileData()
        {
            delete _buff;
            _buff = nullptr;
        }

    public:
        char* Buffer() const { return _buff; }
        size_t Size() const { return _size; }

        bool Load(const char* file)
        {
            std::ifstream fs;
            fs.open(file, std::ios::in);
            if (!fs.is_open())
                return false;

            size_t size = 0;
            fs.seekg(0, std::ios::end);
            size = fs.tellg();
            fs.seekg(0, std::ios::beg);

            if (size >= 3)  // skip bom header
            {
                char bom[3] = {0};
                fs.read(bom, 3);
                if ((uint8_t)bom[0] == 0xEF && (uint8_t)bom[1] == 0xBB && (uint8_t)bom[2] == 0xBF)
                    size -= 3;
                else
                    fs.seekg(0, std::ios::beg);
            }

            _buff = new char[size + 1];
            if (_buff == nullptr)
                return false;

            _size = size;
            fs.read(_buff, size);
            fs.close();

            return true;
        }

    protected:
        char* _buff;
        size_t _size;
    };

    template <class Ty, class Alloc = std::allocator<_Ty> >
    class TabDataList
    {
    public:
        typedef Ty value_type;
        typedef Alloc allocator;
        typedef std::vector<value_type, allocator> vector_data;

    public:
        bool Load(const char* file)
        {
            TabFileData fd;
            if (!fd.Load(file))
                return false;

            return Load(fd.Buffer(), fd.Size(), file);
        }

        bool Load(const char* data, size_t size, const char* chunk = nullptr)
        {
            return Tab::Load(data, size, _data, chunk);
        }

        size_t Size() const { return _data.size(); }

        const value_type& operator[] (int index) const
        {
            assert(index < (int)Size());
            return _data[index];
        }

    protected:
        vector_data _data;
    };


    /*
    * Keyfn example
     struct Keyfn
     {
        key_type Key(value_type& value) { return value.key; }
     };
    */

    /*
     * 配置数据map表
     * Keyfn: 通过value获取key值
    */
    template <
        class Kty, class Vty, class Keyfn,
        class Less = std::less<Kty>,
        class Alloc = std::allocator<std::pair<const Kty, Vty> >
    >
        class TabDataMap
    {
    public:
        typedef Kty key_type;
        typedef Vty value_type;
        typedef const value_type* const_pointer;
        typedef Keyfn key_fn;
        typedef Less less;
        typedef Alloc allocator;
        typedef std::map<key_type, value_type, less, allocator> map_data;

    public:
        bool Load(const char* file)
        {
            TabFileData fd;
            if (!fd.Load(file))
                return false;

            return Load(fd.Buffer(), fd.Size(), file);
        }

        bool Load(const char* data, size_t size, const char* chunk = nullptr)
        {
            std::vector<value_type> vecData;
            if (!Tab::Load(data, size, vecData, chunk))
                return false;

            key_fn kfn;
            for (auto it = vecData.begin(); it != vecData.end(); ++it)
            {
                if (!Filter(*it))
                    continue;

                if (!_data.insert(std::make_pair(kfn.Key(*it), *it)).second)
                    utility::Log("chunk:%s TabDataMap key conflict", chunk ? chunk : "");
            }

            return true;
        }

        const_pointer Find(const key_type& key) const
        {
            auto it = _data.find(key);
            if (it == _data.end())
                return nullptr;
            return &(it->second);
        }

        template <class Func>
        bool Traverse(Func&& func) const
        {
            for (auto it = _data.cbegin(); it != _data.cend();)
            {
                const value_type& val = it->second;
                ++it;

                if (!func(val))
                    return false;
            }
            return true;
        }

        size_t Size() const { return _data.size(); }

    protected:
        virtual bool Filter(value_type& value) { return true; }

    protected:
        map_data _data;
    };
}
