#include "Scanner.h"
#include <fstream>
#include <cassert>
#include "Driver.h"
#include "location.hh"

namespace Cfg
{
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
        while (!m_fileStack.empty())
            m_fileStack.pop();
    }

    const std::string& Scanner::File() const
    {
        if (m_fileStack.empty())
            return _DUMMY_FILE.file;
        return m_fileStack.top()->file;
    }

    location& Scanner::Location()
    {
        if (m_fileStack.empty())
            return _DUMMY_FILE.loc;
        return m_fileStack.top()->loc;
    }

    bool Scanner::Include(const std::string& file)
    {
        return Push(file);
    }

    bool Scanner::EndOfFile()
    {
        yypop_buffer_state();
        return Pop();
    }

    bool Scanner::Push(const std::string& file)
    {
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

        m_fileStack.push(ptr);

        return true;
    }

    bool Scanner::Pop()
    {
        if (m_fileStack.empty())
            return false;

        m_fileStack.pop();
        if (m_fileStack.empty())
            return false;

        return true;
    }
}
