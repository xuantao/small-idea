#include "Driver.h"
#include "Scanner.h"
#include "Parser.hpp"
#include "Context.h"

CFG_NAMESPACE_BEGIN

Driver::Driver()
    : m_pScanner(nullptr)
    , m_pParser(nullptr)
    , m_pContext(nullptr)
{
}

Driver::~Driver()
{
}

bool Driver::Parse(Context& context, const std::string& file)
{
    m_pScanner = new Scanner(*this);
    m_pParser = new Parser(*this);
    m_pContext = &context;

    if (!m_pScanner->Init(file))
        return false;

    m_pParser->parse();

    m_pScanner = nullptr;
    m_pParser = nullptr;
    return true;
}

bool Driver::Parse(Context& context, const std::vector<std::string>& files)
{
    m_pScanner = new Scanner(*this);
    m_pParser = new Parser(*this);
    m_pContext = &context;

    if (!m_pScanner->Init(files))
        return false;

    m_pParser->parse();

    m_pScanner = nullptr;
    m_pParser = nullptr;
    return true;
}

void Driver::Error(const location& loc, const std::string& msg)
{
    std::cerr << "error: loc:" << loc << "msg:" << msg << std::endl;
}

CFG_NAMESPACE_END
