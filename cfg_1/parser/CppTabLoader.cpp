#include "CppTabLoader.h"
#include "Utility.h"

CFG_NAMESPACE_BEGIN

namespace detail
{
    bool CppTabLoader::Declare(IStructType* sType, std::ostream& stream)
    {
        return true;
    }

    bool CppTabLoader::OnStart(const IStructType* sType)
    {
        return true;
    }

    bool CppTabLoader::OnEnd()
    {
        return true;
    }

    bool CppTabLoader::OnVar(const IVariate* var, const IRawType* rType,
        const std::string& title, const std::string& path)
    {
        switch (rType->Raw())
        {
        case RawCategory::Bool:
            _TAB_ << "pTab->GetBoolean(line, \"" << title << "\" , out." << path << ", &out." << path << ");" << std::endl;
            break;
        case RawCategory::Int:
            _TAB_ << "pTab->GetInteger(line, \"" << title << "\" , out." << path << ", &out." << path << ");" << std::endl;
            break;
        case RawCategory::Float:
            _TAB_ << "pTab->GetFloat(line, \"" << title << "\" , out." << path << ", &" << path << ");" << std::endl;
            break;
        case RawCategory::String:
            _TAB_ << "pTab->GetString(line, \"" << title << "\" , out." << path << ".c_str(), cBuffer, CFG_STR_BUFFER_SIZE);" << std::endl;
            _TAB_ << "out." << path << " = cBuffer;" << std::endl;
            break;
        }
        return true;
    }

    bool CppTabLoader::OnVar(const IVariate* var, const IEnumType* eType,
        const std::string& title, const std::string& path)
    {
        std::string tyName = utility::Contact(utility::Absolute(eType), "::");
        _TAB_ << "pTab->GetString(line, \"" << title << "\" , \"\", cBuffer, CFG_STR_BUFFER_SIZE);" << std::endl <<
            _TAB_EX_(0) << "if (!Enum::ToEnum(cBuffer, out." << path << "))" << std::endl <<
            _TAB_EX_(0) << "{" << std::endl <<
            _TAB_EX_(1) << "if (utility::Convert(cBuffer, nTemp) && Enum::ToString((" << tyName << ")nTemp))" << std::endl <<
            _TAB_EX_(2) << "out." << path << " = (" << tyName << ")nTemp;" << std::endl <<
            _TAB_EX_(1) << "else" << std::endl <<
            _TAB_EX_(2) << "; //TODO: log error" << std::endl <<
            _TAB_EX_(0) << "}" << std::endl;
        return true;
    }

    bool CppTabLoader::OnVar(const IVariate* var, const IRawType* rType,
        const std::string& title, const std::string& path, int length)
    {
        if (rType->Raw() == RawCategory::String)
        {
            _OUTS_ <<
                _TAB_EX_(0) << "pTab->GetString(line, \"" << title << "\" , \"\", cBuffer, CFG_STR_BUFFER_SIZE);" << std::endl <<
                _TAB_EX_(0) << "out." << path << " = utility::Split(cBuffer, \",\");" << std::endl;
        }
        else
        {
            _OUTS_ <<
                _TAB_EX_(0) << "pTab->GetString(line, \"" << title << "\" , \"\", cBuffer, CFG_STR_BUFFER_SIZE);" << std::endl <<
                _TAB_EX_(0) << "vecStr = utility::Split(cBuffer, \",\");" << std::endl;

            if (length <= 0)
            {
                _OUTS_ <<
                    _TAB_EX_(0) << "out." << path << ".resize(vecStr.size());" << std::endl <<
                    _TAB_EX_(0) << "for (size_t i = 0; i < vecStr.size(); ++i)" << std::endl;
            }
            else
            {
                _OUTS_ <<
                    _TAB_EX_(0) << "for (size_t i = 0; i < std::min(vecStr.size(), out." << path << ".size()); ++i)" << std::endl;
            }

            _OUTS_ <<
                _TAB_EX_(0) << "{" << std::endl <<
                _TAB_EX_(1) << "if (!utility::Convert(vecStr[i].c_str(), out." << path << "[i]))" << std::endl <<
                _TAB_EX_(2) << "; //TODO: log error" << std::endl <<
                _TAB_EX_(0) << "}" << std::endl;
        }
        return true;
    }

    bool CppTabLoader::OnVar(const IVariate* var, const IEnumType* eType,
        const std::string& title, const std::string& path, int length)
    {
        std::string tyName = utility::Contact(utility::Absolute(eType), "::");
        _OUTS_ <<
            _TAB_EX_(0) << "pTab->GetString(line, \"" << title << "\" , \"\", cBuffer, CFG_STR_BUFFER_SIZE);" << std::endl <<
            _TAB_EX_(0) << "vecStr = utility::Split(cBuffer, \",\");" << std::endl;

        if (length <= 0)
        {
            _OUTS_ <<
                _TAB_EX_(0) << "out." << path << ".resize(vecStr.size());" << std::endl <<
                _TAB_EX_(0) << "for (size_t i = 0; i < vecStr.size(); ++i)" << std::endl;
        }
        else
        {
            _OUTS_ <<
                _TAB_EX_(0) << "for (size_t i = 0; i < std::min(vecStr.size(), out." << path << ".size()); ++i)" << std::endl;
        }

        _OUTS_ <<
            _TAB_EX_(0) << "{" << std::endl <<
            _TAB_EX_(1) << "if (!Enum::ToEnum(vecStr[i].c_str(), out." << path << "[i]))" << std::endl <<
            _TAB_EX_(1) << "{" << std::endl <<
            _TAB_EX_(2) << "if (utility::Convert(vecStr[i].c_str(), nTemp) && Enum::ToString((" << tyName << ")nTemp))" << std::endl <<
            _TAB_EX_(3) << "out." << path << "[i] = (" << tyName << ")nTemp;" << std::endl <<
            _TAB_EX_(2) << "else" << std::endl <<
            _TAB_EX_(3) << "; //TODO: log error" << std::endl <<
            _TAB_EX_(1) << "}" << std::endl <<
            _TAB_EX_(0) << "}" << std::endl;
        return true;
    }
}

CFG_NAMESPACE_END
