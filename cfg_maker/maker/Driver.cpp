#include "Driver.h"
#include "Scanner.h"
#include "Context.h"
#pragma warning( push )
#pragma warning( disable : 4146 )
#pragma warning( disable : 4065 )
#include "Parser.hpp"
#pragma warning( pop )

CFG_NAMESPACE_BEGIN

Driver::Driver()
    : _errorNum(0)
    , _warNum(0)
    , _scanner(nullptr)
    , _parser(nullptr)
    , _context(nullptr)
{
}

Driver::~Driver()
{
}

bool Driver::Parse(Context& context, const std::string& file)
{
    _scanner = new Scanner(*this);
    _parser = new Parser(*this);
    _context = &context;

    if (!_scanner->Init(file))
        return false;

    _parser->parse();

    _scanner = nullptr;
    _parser = nullptr;
    return true;
}

bool Driver::Parse(Context& context, const std::string& path, const std::vector<std::string>& files)
{
    _scanner = new Scanner(*this);
    _parser = new Parser(*this);
    _context = &context;

    if (!_scanner->Init(path, files))
        return false;

    _parser->parse();

    _scanner = nullptr;
    _parser = nullptr;
    return _errorNum == 0;
}

void Driver::Error(const location& loc, const std::string& msg)
{
    std::cerr << "error: loc:" << loc << "msg:" << msg << std::endl;
    ++_errorNum;
}

CFG_NAMESPACE_END
