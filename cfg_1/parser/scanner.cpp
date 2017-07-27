#include "Scanner.h"
#include "Driver.h"
#include "Context.h"
#include "location.hh"
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
        std::string file;
        std::ifstream stream;
    };
}

static detail::ScanningFile _DUMMY_FILE;

Scanner::Scanner(Driver& driver)
    : CfgFlexLexer()
    , m_driver(driver)
{
}

Scanner::~Scanner()
{
}

bool Scanner::Init(const std::string& file)
{
    return Push(file);
}

void Scanner::UnInit()
{
    m_fileStack.clear();
}

const std::string& Scanner::File() const
{
    if (m_fileStack.empty())
        return _DUMMY_FILE.file;
    return m_fileStack.back()->file;
}

location& Scanner::Location()
{
    if (m_fileStack.empty())
        return _DUMMY_FILE.loc;
    return m_fileStack.back()->loc;
}

bool Scanner::Include(const std::string& file)
{
    if (!Push(file))
        return false;

    m_driver.GetContext()->OnIncludeBegin(file);
    return true;
}

bool Scanner::EndOfFile()
{
    yypop_buffer_state();

    if (!Pop())
        return false;

    m_driver.GetContext()->OnIncludeEnd();
}

void Scanner::Unrecognized(char c)
{
    std::cerr << "unknown character:" << c << std::endl;
}

bool Scanner::Push(const std::string& file)
{
    for (auto it = m_fileStack.begin(); it != m_fileStack.end(); ++it)
    {
        if ((*it)->file == file)
        {
            std::cerr << "include file already include " << file << std::endl;
            return false;
        }
    }

    FilePtr ptr = std::make_shared<detail::ScanningFile>();
    ptr->file = file;
    ptr->stream.open(file);
    ptr->loc.initialize(&ptr->file);

    if (!ptr->stream.is_open())
    {
        std::cerr << "open file:" << file << " failed" << std::endl;
        return false;
    }

    if (m_fileStack.empty())
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

    m_fileStack.push_back(ptr);

    return true;
}

bool Scanner::Pop()
{
    if (m_fileStack.empty())
        return false;

    m_fileStack.pop_back();
    if (m_fileStack.empty())
        return false;

    return true;
}

CFG_NAMESPACE_END
