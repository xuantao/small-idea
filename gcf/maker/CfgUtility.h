/*
 * 基础工具
*/
#pragma once
#include <string>
#include <vector>

#define CFG_INVALID_ENUM    -1000000
#define CFG_STR_BUFFER_SIZE  2048

namespace cfg
{
    namespace utility
    {
        bool Convert(const std::string& str, bool& out);
        bool Convert(const std::string& str, int& out);
        bool Convert(const std::string& str, float& out);
        std::vector<std::string> Split(const std::string& str, const std::string& s);
    }
}
