/*
 * 用于序列化的相关接口和辅助函数
*/
#pragma once

#include "ISerialize.h"
#include <cstdio>
#include <cassert>

namespace serialize
{
    namespace utility
    {
        /*
         * read functions
        */
        bool Read(IReader* reader, bool& val, const char* name = nullptr);
        bool Read(IReader* reader, int8_t& val, const char* name = nullptr);
        bool Read(IReader* reader, int32_t& val, const char* name = nullptr);
        bool Read(IReader* reader, int64_t& val, const char* name = nullptr);
        bool Read(IReader* reader, float& val, const char* name = nullptr);
        bool Read(IReader* reader, double& val, const char* name = nullptr);
        bool Read(IReader* reader, std::string& val, const char* name = nullptr);
        bool Read(IReader* reader, std::vector<bool>& vec, const char* name = nullptr);

        template <class Ty>
        bool Read(IReader* reader, std::vector<Ty>& vec, const char* name = nullptr)
        {
            int size = 0;
            if (!reader->ArrayBegin(size, name))
                return false;

            vec.resize(size);
            for (int i = 0; i < size; ++i)
                if (!Read(reader, vec[i]))
                    return false;

            return reader->ArrayEnd();
        }

        template <class Ty, size_t N>
        bool Read(IReader* reader, std::array<Ty, N>& arr, const char* name = nullptr)
        {
            for (size_t i = 0; i < N; ++i)
            {
                char arName[256] = {0};
                std::snprintf(arName, 255, "%s_%d", name ? name : "", (int)i + 1);
                if (!Read(reader, arr[i], name))
                    return false;
            }

            return true;
        }

        //template <class Ky, class Ty>
        //bool Read(IReader* reader, std::map<Ky, Ty>& map)
        //{
        //    int size = 0;
        //    if (!Read(reader, size)) return false;
        //    if (size < 0) return false;

        //    for (size_t i = 0; i < N; ++i)
        //    {
        //        std::pair<Ky, Ty> pair;
        //        if (!Read(reader, pair.first)) return false;
        //        if (!Read(reader, pair.second)) return false;

        //        map.insert(std::move(pair));
        //    }
        //    return true;
        //}

        /*
         * write functions
        */
        bool Write(IWriter* writer, bool val, const char* name = nullptr);
        bool Write(IWriter* writer, int8_t val, const char* name = nullptr);
        bool Write(IWriter* writer, int32_t val, const char* name = nullptr);
        bool Write(IWriter* writer, int64_t val, const char* name = nullptr);
        bool Write(IWriter* writer, float val, const char* name = nullptr);
        bool Write(IWriter* writer, double val, const char* name = nullptr);
        bool Write(IWriter* writer, const std::string& val, const char* name = nullptr);

        template <class Ty>
        bool Write(IWriter* writer, const std::vector<Ty>& vec, const char* name = nullptr)
        {
            if (!writer->ArrayBegin((int)vec.size(), name))
                return false;

            for (const auto& val : vec)
                if (!Write(writer, val))
                    return false;

            return writer->ArrayEnd();
        }

        template <class Ty, size_t N>
        bool Write(IWriter* writer, const std::array<Ty, N>& arr, const char* name = nullptr)
        {
            for (size_t i = 0; i < arr.size(); ++i)
            {
                char arName[256] = {0};
                std::snprintf(arName, 255, "%s_%d", name ? name : "", (int)i + 1);
                if (!Write(writer, arr[i], arName))
                    return false;
            }

            return true;
        }

        //template <class Ky, class Ty>
        //bool Write(IWriter* writer, const std::map<Ky, Ty>& map)
        //{
        //    if (!Write(writer, (int)map.size()))
        //        return false;

        //    for (const auto& pair : map)
        //    {
        //        if (!Write(writer, pair.first))
        //            return false;
        //        if (!Write(writer, pair.second))
        //            return false;
        //    }

        //    return true;
        //}
    }
}
