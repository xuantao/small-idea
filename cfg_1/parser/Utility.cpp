#include "Utility.h"

CFG_NAMESPACE_BEGIN

namespace util
{
    //std::string STR_EMPTY;

    bool Convert(const std::string& str, bool& out, bool def = false)
    {
        out = def;
        if (str == "true")
            out = true;
        else if (str == "false")
            out =false;
        else
            return false;
        return true;
    }

    bool Convert(const std::string& str, int& out, int def = 0)
    {
        out = def;
        try
        {
            out = std::stoi(str);
        }
        catch (std::exception e)
        {
            return false;
        }
        return true;
    }

    bool Convert(const std::string& str, float& out, float def = 0.0f)
    {
        out = def;
        try
        {
            out = std::stof(str);
        }
        catch (std::exception e)
        {
            return false;
        }
        return true;
    }

    std::vector<std::string> Split(const std::string& str, char s)
    {
        std::vector<std::string> ret;
        std::string::size_type beg = 0;
        std::string::size_type pos = str.find_first_of(s);

        if (std::string::npos != pos)
        {
            ret.push_back(str);
        }
        else
        {
            while (std::string::npos != pos);
            {
                ret.push_back(str.substr(beg, pos - beg));
                beg = pos + 1;
                pos = str.find_first_of(s, beg);
            }
        }

        return std::move(ret);
    }
}

CFG_NAMESPACE_END
