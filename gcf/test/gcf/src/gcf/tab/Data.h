#pragma once
#include "Parser.h"
#include "FileData.h"
#include "TextLoader.h"
#include "BinaryLoader.h"
#include <vector>
#include <map>
#include <cassert>

namespace tab
{
    template <class Ty>
    class DataBase
    {
    public:
        bool LoadText(const char* file, int startLine = 3, bool hasDefault = true)
        {
            FileData fd;
            if (!fd.Load(file, true))
                return false;

            return LoadText(fd.Data(), startLine, hasDefault);
        }

        bool LoadText(char* data, int startLine = 3, bool hasDefault = true)
        {
            TextLoader<Ty> loader;
            if (!loader.Setup(data, startLine, hasDefault))
                return false;

            return Load(loader);
        }

        bool LoadBinary(const char* file)
        {
            FileData fd;
            if (!fd->Load(file, false))
                return false;

            return LoadBinary(fd.Data(), fd.Size());
        }

        bool LoadBinary(const void* data, size_t size)
        {
            BinaryLoader<Ty> loader;
            if (!loader.Setup(data, size))
                return false;

            return Load(loader);
        }

    public:
        virtual bool Load(Loader<Ty>& loader) = 0;
    };

    template <class Ty>
    class DataList : public DataBase<Ty>
    {
    public:
        virtual bool Load(Loader<Ty>& loader)
        {
            while (true)
            {
                Ty val;
                if (loader.Load(val))
                    _data.push_back(val);
                else
                    break;
            }
            return true;
        }

        const std::vector<Ty>& Data() const
        {
            return _data;
        }
    protected:
        std::vector<Ty> _data;
    };

    template <typename Ky, class Ty>
    class DataMap : public DataBase<Ty>
    {
    public:
        const Ty* Get(const Ky& key) const
        {
            auto it = _data.find(key);
            if (it == _data.cend())
                return nullptr;

            return &(it->second);
        }

    public:
        virtual bool Load(Loader<Ty>& loader)
        {
            while (true)
            {
                Ty val;
                if (!loader.Load(val))
                    break;

                if (!Filter(val))
                    continue;

                auto pd = _data.insert(std::make_pair(Key(val), val));
                assert(pd.second);
            }
            return true;
        }

    protected:
        virtual Ky Key(Ty& val) = 0;
        virtual bool Filter(Ty& val) { return true; }

    protected:
        std::map<Ky, Ty> _data;
    };
}
