/*
 * configuration file parser common define
 * 2017-07-22
*/
#pragma once

#include <cassert>
#include <string>
#include <vector>
#include <type_traits>
#include <algorithm>

#define CFG_NAMESPACE       cfg
#define CFG_NAMESPACE_BEGIN namespace CFG_NAMESPACE {
#define CFG_NAMESPACE_END   }
#define CFG_NAMESPACE_USING using namespace CFG_NAMESPACE
#define CFG_NAMESPACE_REF   CFG_NAMESPACE::

#define _DEF_STREAM_ &std::cerr
#define _OUTS_ (*_stream)
#define _TAB_ utility::Tab(*_stream, _tab)
#define _TAB_EX_(ex) std::string((_tab + ex) * 4, ' ')

#define ERROR_NOT_ALLOW std::cerr << "not allow file:" << __FILE__ << " line:" << __LINE__ << std::endl

CFG_NAMESPACE_BEGIN

// raw type name
#define TYPE_BOOL   "bool"
#define TYPE_INT    "int"
#define TYPE_FLOAT  "float"
#define TYPE_STRING "string"

enum class RawCategory
{
    Bool,
    Int,
    Float,
    String
};

enum class TypeCategory
{
    Raw,
    Enum,
    Struct,
    Array,
    Scope,
};

enum class ValueCategory
{
    Raw,
    Ref,
};

enum class BlockCategory
{
    Type,
    Var,
    CopySrc,
    Comment,
};

enum class CfgCategory
{
    None,
    Tab,
    Json,
};

class IType;
class ITypeSet;
class IVarSet;
class IVariate;
class IValue;
class IBlock;
class IFileData;
class IExporter;

CFG_NAMESPACE_END
