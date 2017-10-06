#pragma once
#include "Parser.h"
#include "FileData.h"
#include "Loader.h"
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

            if (loader.HashCode() != Ty::HASH_CODE)
                return false;

            return Load(loader);
        }

    protected:
        bool Load(ILoader<Ty>& loader)
        {
            while (true)
            {
                Ty val;
                if (!loader.Load(val))
                    break;

                if (!OnLoad(val))
                    return false;
            }

            return true;
        }

        virtual bool OnLoad(Ty& val) = 0;
    };

    template <class Ty>
    class DataList : public DataBase<Ty>
    {
    public:
        const std::vector<Ty>& Datas() const
        {
            return _data;
        }

    protected:
        virtual bool OnLoad(Ty& val)
        {
            if (!Filter(val))
                return true;

            _data.push_back(val);
            return true;
        }

        virtual bool Filter(Ty& val) { return true; }
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

        const std::map<Ky, Ty>& Datas() const
        {
            return _data;
        }

    protected:
        virtual bool OnLoad(Ty& val)
        {
            if (!Filter(val))
                return true;

            auto pd = _data.insert(std::make_pair(Key(val), val));
            assert(pd.second);
            return true;
        }

    protected:
        virtual Ky Key(Ty& val) = 0;
        virtual bool Filter(Ty& val) { return true; }

    protected:
        std::map<Ky, Ty> _data;
    };
}
