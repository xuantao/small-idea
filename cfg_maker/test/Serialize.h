/*
 * 用于序列化的相关接口和辅助函数
*/
#pragma once

#include <stdint.h>
#include <string>
#include <array>
#include <vector>
#include <map>

namespace serialize
{
    class IReader
    {
    public:
        virtual ~IReader() {}

    public:
        virtual bool StructBegin(uint32_t code) = 0;
        virtual bool StructEnd() = 0;
        virtual bool Read(bool& val) = 0;
        virtual bool Read(int32_t& val) = 0;
        virtual bool Read(uint32_t& val) = 0;
        virtual bool Read(float& val) = 0;
        virtual bool Read(std::string& val) = 0;
    };

    class IWriter
    {
    public:
        virtual ~IWriter() {}

    public:
        virtual bool StructBegin(uint32_t code) = 0;
        virtual bool StructEnd() = 0;
        virtual bool Write(bool val) = 0;
        virtual bool Write(int32_t val) = 0;
        virtual bool Write(uint32_t val) = 0;
        virtual bool Write(float val) = 0;
        virtual bool Write(const std::string& val) = 0;
    };

    namespace utility
    {
        /*
         * read functions
        */
        bool Read(IReader* reader, bool& val);
        bool Read(IReader* reader, int32_t& val);
        bool Read(IReader* reader, uint32_t& val);
        bool Read(IReader* reader, float& val);
        bool Read(IReader* reader, std::string& val);

        template <class Ty>
        bool Read(IReader* reader, std::vector<Ty>& vec)
        {
            int size = 0;
            if (!Read(reader, size)) return false;
            if (size < 0) return false;

            vec.resize(size);
            for (int i = 0; i < size; ++i)
                if (!Read(reader, vec[i]))
                    return false;

            return true;
        }

        template <class Ty, size_t N>
        bool Read(IReader* reader, std::array<Ty, N>& arr)
        {
            for (size_t i = 0; i < N; ++i)
                if (!Read(reader, arr[i]))
                    return false;

            return true;
        }

        template <class Ky, class Ty>
        bool Read(IReader* reader, std::map<Ky, Ty>& map)
        {
            int size = 0;
            if (!Read(reader, size)) return false;
            if (size < 0) return false;

            for (size_t i = 0; i < N; ++i)
            {
                std::pair<Ky, Ty> pair;
                if (!Read(reader, pair.first)) return false;
                if (!Read(reader, pair.second)) return false;

                map.insert(std::move(pair));
            }
            return true;
        }

        /*
         * write functions
        */
        bool Write(IWriter* writer, bool val);
        bool Write(IWriter* writer, int32_t val);
        bool Write(IWriter* writer, uint32_t val);
        bool Write(IWriter* writer, float val);
        bool Write(IWriter* writer, const std::string& val);

        template <class Ty>
        bool Write(IWriter* writer, const std::vector<Ty>& vec)
        {
            if (!Write(writer, (int)vec.size()))
                return false;

            for (const auto& val : vec)
                if (!Write(writer, val))
                    return false;

            return true;
        }

        template <class Ty, size_t N>
        bool Write(IWriter* writer, const std::array<Ty, N>& arr)
        {
            for (const auto& val : arr)
                if (!Write(writer, val))
                    return false;

            return true;
        }

        template <class Ky, class Ty>
        bool Write(IWriter* writer, const std::map<Ky, Ty>& map)
        {
            if (!Write(writer, (int)map.size()))
                return false;

            for (const auto& pair : map)
            {
                if (!Write(writer, pair.first))
                    return false;
                if (!Write(writer, pair.second))
                    return false;
            }

            return true;
        }
    }
}
