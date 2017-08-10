#include "Scanner.h"
#include "Driver.h"
#include "Context.h"
#include "location.hh"
#include "Utility.h"
#include <fstream>
#include <cassert>

CFG_NAMESPACE_BEGIN

namespace detail
{
    class ScanningFile
    {
    public:
        ScanningFile() : loc(nullptr)
        {
        }

        ~ScanningFile()
        {
            if (stream.is_open())
                stream.close();
        }

    public:
        location loc;
        std::string path;
        std::string file;
        std::ifstream stream;
    };
}

static detail::ScanningFile _DUMMY_FILE;

Scanner::Scanner(Driver& driver)
    : CfgFlexLexer()
    , _driver(driver)
{
}

Scanner::~Scanner()
{
}

bool Scanner::Init(const std::string& file)
{
    return Push(file);
}

bool Scanner::Init(const std::string& path, const std::vector<std::string>& files)
{
    if (files.empty())
        return false;

    _rootPath = path;
    _files.reserve(files.size());
    _files.assign(files.rbegin(), files.rend());

    return NextFile();
}

void Scanner::UnInit()
{
    _desc = "";
    _stack.clear();
    _files.clear();
    _parsed.clear();
}

const std::string& Scanner::File() const
{
    if (_stack.empty())
        return _DUMMY_FILE.file;
    return _stack.back()->file;
}

location& Scanner::Location()
{
    if (_stack.empty())
        return _DUMMY_FILE.loc;
    return _stack.back()->loc;
}

bool Scanner::Include(const std::string& file)
{
    std::string real = utility::AbsolutePath(
        utility::ContactPath(_stack.back()->path, file));

    if (_driver.GetContext()->OnIncludeBegin(_stack.back()->path, file))
    {
        if (!Push(real))
        {
            _driver.GetContext()->OnIncludeEnd();
            return false;
        }
    }

    return true;
}

bool Scanner::EndOfFile()
{
    yypop_buffer_state();

    Pop();
    if (_stack.empty())
    {
        _driver.GetContext()->OnParseEnd();
        NextFile();
    }
    else
    {
        _driver.GetContext()->OnIncludeEnd();
    }

    return !_stack.empty();
}

void Scanner::Unrecognized(char c)
{
    _driver.Error("unknown character:{0}", c);
}

bool Scanner::NextFile()
{
    while (!_files.empty())
    {
        std::string file(std::move(_files.back()));
        _files.pop_back();

        if (_parsed.find(file) != _parsed.end())
            continue;   // already scanned

        if (!Push(file))
            continue;   // parse failed

        _driver.GetContext()->OnParseBegin(file);
        break;
    }

    return true;
}

bool Scanner::Push(const std::string& file)
{
    _parsed.insert(file);

    std::string path;
    utility::SplitPath(file, &path);

    FilePtr ptr = std::make_shared<detail::ScanningFile>();
    ptr->file = file;
    ptr->path = path;
    ptr->loc.initialize(&ptr->file);
    ptr->stream.open(utility::ContactPath(_rootPath, file));

    if (!ptr->stream.is_open())
    {
        _driver.Error("open file: {0} failed", file);
        return false;
    }

    if (_stack.empty())
    {
        switch_streams(&(ptr->stream), nullptr);
    }
    else
    {
        yy_buffer_state* buffer = yy_create_buffer(ptr->stream, YY_BUF_SIZE);
        if (buffer == nullptr)
        {
            _driver.Error("create scanner buffer faile for file:{0}", file);
            return false;
        }
        yypush_buffer_state(buffer);
    }

    _stack.push_back(ptr);

    return true;
}

bool Scanner::Pop()
{
    if (!_stack.empty())
        _stack.pop_back();

    return true;
}

CFG_NAMESPACE_END
