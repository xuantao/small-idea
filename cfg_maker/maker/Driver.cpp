﻿#include "Driver.h"
#include "Scanner.h"
#include "Context.h"
#pragma warning( push )
#pragma warning( disable : 4146 )
#pragma warning( disable : 4065 )
#include "Parser.hpp"
#pragma warning( pop )

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

bool Driver::Parse(Context& context, const std::string& path, const std::vector<std::string>& files)
{
    m_pScanner = new Scanner(*this);
    m_pParser = new Parser(*this);
    m_pContext = &context;

    if (!m_pScanner->Init(path, files))
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
