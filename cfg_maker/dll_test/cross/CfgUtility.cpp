#include "CfgUtility.h"
#include <cstdarg>

namespace cfg
{
    namespace utility
    {
        static void LogWrap(const char* msg)
        {
            printf(msg);
        }

        static LogCallback sLogger = LogWrap;

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

        void SetLogger(LogCallback logger)
        {
            sLogger = logger;
        }

        LogCallback GetLogger()
        {
            return sLogger;
        }

        void Log(const char* fmt, ...)
        {
            if (!sLogger)
                return;

            char buffer[2048] = { 0 };
            va_list args;

            va_start(args, fmt);
            vsnprintf(buffer, 2048, fmt, args);
            va_end(args);

            sLogger(buffer);
        }
    }
}
