#include "TabCreater.h"
#include "Utility.h"
#include "ValueUtil.h"
#include <fstream>
#include <iostream>
#include <map>

CFG_NAMESPACE_BEGIN

bool _load(const std::string& file, std::vector<std::vector<std::string> >& data)
{
    std::string line;
    std::ifstream in;
    in.open(file);
    if (!in.is_open())
        return false;

    size_t count = 0;
    data.clear();
    while (std::getline(in, line))
    {
        std::vector<std::string> ld = utility::Split(line, "\t");
        if (ld.empty())
            continue;

        if (data.empty())
        {
            count = ld.size();
            data.resize(ld.size());
        }

        for (size_t i = 0; i < count; ++i)
        {
            if (i < ld.size())
                data[i].push_back(ld[i]);
            else
                data[i].push_back("");
        }
    }

    return true;
}

static bool _Save(const std::string& file, std::vector<std::vector<std::string> >& data)
{
    std::ofstream out;
    out.open(file);
    if (!out.is_open())
        return false;

    for (size_t i = 0; i < data[0].size(); ++i)
    {
        for (size_t j = 0; j < data.size(); ++j)
        {
            if (j) out << "\t";
            out << data[j][i];
        }
        out << "\n";
    }
    return true;
}

TabCreater::TabCreater()
{
}

void TabCreater::SetPath(const std::string& path)
{
    _path = path;
}

bool TabCreater::OnStart(const IStructType* sType)
{
    if (_path.empty())
        _file = sType->Name() + ".tab";
    else
        _file = _path + "/" + sType->Name() + ".tab";
    return true;
}

bool TabCreater::OnEnd()
{

    if (utility::IsFile(_file))
        Merge();
    else
        Create();

    _file.clear();
    _title.clear();
    _type.clear();
    _desc.clear();
    _def.clear();
    return true;
}

bool TabCreater::OnVar(const IVariate* var, const IRawType* rType,
    const std::string& title, const std::string& path)
{
    _title.push_back(title);
    _type.push_back(rType->Name());
    _desc.push_back(var->Desc());

    if (var->Value())
        _def.push_back(value_util::ToString(var->Value()));
    else
        _def.push_back(value_util::DefValue(rType->RawCat()));
    return true;
}

bool TabCreater::OnVar(const IVariate* var, const IEnumType* eType,
    const std::string& title, const std::string& path)
{
    _title.push_back(title);
    _type.push_back(eType->Name());
    _desc.push_back(var->Desc());

    if (var->Value())
        _def.push_back(value_util::ToString(var->Value()));
    else
        _def.push_back("");
    return true;
}

bool TabCreater::OnVar(const IVariate* var, const IRawType* rType,
    const std::string& title, const std::string& path, int length)
{
    _title.push_back(title);
    if (length > 0)
        _type.push_back(rType->Name() + "[" + std::to_string(length) + "]");
    else
        _type.push_back(rType->Name() + "[" + "]");

    _desc.push_back(var->Desc());
    _def.push_back(value_util::DefValue(rType->RawCat()));
    return true;
}

bool TabCreater::OnVar(const IVariate* var, const IEnumType* eType,
    const std::string& title, const std::string& path, int length)
{
    _title.push_back(title);
    if (length > 0)
        _type.push_back(eType->Name() + "[" + std::to_string(length) + "]");
    else
        _type.push_back(eType->Name() + "[" + "]");

    _desc.push_back(var->Desc());
    _def.push_back("");
    return true;
}

void TabCreater::Create()
{
    if (!utility::IsDir(_path))
        utility::CreateDir(_path);

    std::ofstream fs;
    fs.open(_file);
    if (!fs.is_open())
    {
        std::cerr << "open file failed " << _file << std::endl;
        return;
    }

    fs <<
        utility::Contact(_title, "\t") << std::endl <<
        utility::Contact(_type, "\t") << std::endl <<
        utility::Contact(_desc, "\t") << std::endl <<
        utility::Contact(_def, "\t") << std::endl;
}

void TabCreater::Merge()
{
    std::vector<std::vector<std::string> > data;
    if (!_load(_file, data) || data.size() <= 4)
    {
        Create();
        return;
    }

    size_t lineNum = data[0].size();
    std::map<std::string, size_t> titleIdx;
    for (size_t i = 0; i < _title.size(); ++i)
        titleIdx[_title[i]] = i;

    // copy & set "deleted"
    for (size_t i = 0; i < data.size(); ++i)
    {
        auto it = titleIdx.find(data[i][0]);
        if (it == titleIdx.end())
        {
            data[i][3] = "deleted";
        }
        else
        {
            data[i][1] = _type[it->second];
            data[i][2] = _desc[it->second];
            data[i][3] = _def[it->second];
        }
    }

    /* inset new column */
    int lastIdx = 0;
    for (size_t i = 0; i < _title.size(); ++i)
    {
        int idx = -1;
        for (int j = 0; j < (int)data.size(); ++j)
        {
            if (_title[i] == data[j][0])
            {
                idx = j;
                break;
            }
        }

        if (idx == -1)
        {
            data.insert(data.begin() + lastIdx, std::vector<std::string>());
            data[lastIdx].resize(lineNum);
            data[lastIdx][0] = _title[i];
            data[lastIdx][1] = _type[i];
            data[lastIdx][2] = _desc[i];
            data[lastIdx][3] = _def[i];
            ++lastIdx;
        }
        else
        {
            lastIdx = idx + 1;
        }
    }

    if (!_Save(_file, data))
        std::cerr << "open file failed " << _file << std::endl;
}

CFG_NAMESPACE_END
