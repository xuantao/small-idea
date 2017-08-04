#include "CfgUtility.h"

namespace cfg
{
    namespace utility
    {
        static FnLogger sLogger = nullptr;

        bool Convert(const std::string& str, bool& out)
        {
            if (str == "true" || str == "True" || str == "TRUE" || str == "1")
                out = true;
            else if (str == "false" || str == "False" || str == "FALSE" || str == "0")
                out = false;
            else
                return false;
            return true;
        }

        bool Convert(const std::string& str, int& out)
        {
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

        bool Convert(const std::string& str, float& out)
        {
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

        std::vector<std::string> Split(const std::string& str, const std::string& s)
        {
            std::vector<std::string> ret;
            std::string::size_type beg = 0;
            std::string::size_type pos = str.find_first_of(s);

            while (std::string::npos != pos)
            {
                ret.push_back(str.substr(beg, pos - beg));
                beg = pos + s.length();
                pos = str.find_first_of(s, beg);
            }

            if (beg < str.length())
                ret.push_back(str.substr(beg));

            return std::move(ret);
        }

        void SetLogger(FnLogger logger)
        {
            sLogger = logger;
        }

        FnLogger GetLogger()
        {
            return sLogger;
        }
    }
}
