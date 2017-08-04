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

bool Scanner::Init(const std::vector<std::string>& files)
{
    if (files.empty())
        return false;

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
    if (!Push(file))
        return false;

    _driver.GetContext()->OnIncludeBegin(file);
    return true;
}

bool Scanner::EndOfFile()
{
    yypop_buffer_state();

    if (Pop())
        _driver.GetContext()->OnIncludeEnd();
    else if (!NextFile())
        return false;

    return true;
}

void Scanner::Unrecognized(char c)
{
    std::cerr << "unknown character:" << c << std::endl;
}

bool Scanner::NextFile()
{
    while (!_files.empty() && !Push(_files.back()))
    {
        _files.pop_back();
    }

    if (_files.empty())
        return false;

    _driver.GetContext()->OnParseBegin(_files.back());
    _files.pop_back();
    return true;
}

bool Scanner::Push(const std::string& file)
{
    std::string path;
    std::string real;
    if (!_stack.empty())
    {
        real = utility::AbsolutePath(_stack.back()->path + "/" + file);
        utility::SplitPath(real, &path);
    }
    else
    {
        real = file;
        utility::SplitPath(file, &path);
    }

    if (_parsed.find(real) != _parsed.end())
    {
        std::cerr << "include file already include " << real << std::endl;
        return false;
    }

    FilePtr ptr = std::make_shared<detail::ScanningFile>();
    ptr->file = real;
    ptr->path = path;
    ptr->stream.open(real);
    ptr->loc.initialize(&ptr->file);

    if (!ptr->stream.is_open())
    {
        std::cerr << "open file:" << real << " failed" << std::endl;
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
            std::cerr << "failed" << std::endl;
            return false;
        }
        yypush_buffer_state(buffer);
    }

    _stack.push_back(ptr);
    _parsed.insert(real);

    return true;
}

bool Scanner::Pop()
{
    if (_stack.empty())
        return false;

    _stack.pop_back();
    if (_stack.empty())
        return false;

    return true;
}

CFG_NAMESPACE_END
