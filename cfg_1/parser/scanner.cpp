#include "Scanner.h"
#include <fstream>
#include <cassert>

namespace Cfg
{
    namespace detail
    {
        class ScanningFile
        {
        public:
            ScanningFile() : lineno(0)
            {
            }

            ~ScanningFile()
            {
                if (stream.is_open())
                    stream.close();
            }

        public:
            std::string file;
            std::ifstream stream;
            int lineno;
        };

        static std::string EMPTY;
    }

    Scanner::Scanner()
        : CfgFlexLexer()
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
            return detail::EMPTY;
        return m_fileStack.top()->file;
    }

    int Scanner::LineNO() const
    {
        return yylineno;
    }

    int Scanner::yywrap()
    {
        return 1;
    }

    void Scanner::Include(const std::string& file)
    {
        Push(file);
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
            m_fileStack.top()->lineno = yylineno;

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

        yylineno = m_fileStack.top()->lineno;
        return true;
    }
}
