#include "scanner.h"
#include <fstream>
#include <cassert>

namespace zh {
    Scanner::Scanner(std::istream* in, std::ostream* out)
        : zhFlexLexer(in, out)
    {
    }

    Scanner::~Scanner()
    {
    }

    void Scanner::set_debug(bool b)
    {
        yy_flex_debug = b;
    }

    void Scanner::Include(const char* lpFile, bool bRelative)
    {
        std::string file(&lpFile[1], strlen(lpFile) - 2);
        std::ifstream* pStream = new std::ifstream();
        pStream->open(file);
        if (!pStream->is_open())
        {
            delete pStream;
            assert(false);
        }

        //yyin = pStream;
        //yypush_buffer_state(yy_create_buffer(yyin, YY_BUFF_SIZE));
    }
}
