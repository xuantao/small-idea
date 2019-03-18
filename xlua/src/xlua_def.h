#pragma once
#include <lua.hpp>
#include <type_traits>

#define XLUA_NAMESPACE_BEGIN    namespace xlua {
#define XLUA_NAMESPACE_END      } // namespace xlua

XLUA_NAMESPACE_BEGIN

namespace detail {
    class GlobalVar;

    template <typename... Tys> struct BaseType { static_assert(sizeof...(Tys) > 1, "not allow multy inherit"); };
    template <> struct BaseType<> { typedef void type; };
    template <typename Ty> struct BaseType<Ty> { typedef Ty type; };
}

class xLuaState;
struct TypeInfo;

typedef int (*LuaFunction)(xLuaState* L);
typedef void (*LuaIndexer)(xLuaState* L, void* obj);
typedef void* (*LuaPointerConvert)(void* obj, const TypeInfo* src_type, const TypeInfo* dst_type);
/* convert shared_ptr object */
typedef bool (*LuaSharedPtrConvert)(void* obj_ptr, const TypeInfo* src_type, void* dst, const TypeInfo* dst_type);

template <typename Ty, typename By>
struct Declare {
    typedef By  super;
    typedef Ty  self;
};

// std::indetity
template <typename Ty>
struct Identity {
    typedef Ty type;
};

class TypeKey {
    friend class detail::GlobalVar;
public:
    bool IsValid() const;

private:
    int index_ = -1;
    int serial_ = 0;
};

enum class ConstValueType {
    kNone,
    kInteger,
    kFloat,
    kString,
};

struct ConstValue {
    ConstValueType type;
    const char* name;

    union {
        int int_val;
        float float_val;
        const char* string_val;
    };
};

struct ConstInfo {
    const char* name;
    const ConstValue* values;
};

enum class MemberType {
    kInvalid = 0,
    kVariate,
    kFunction,
};

struct TypeMember {
    MemberType type;
    const char* name;
    union {
        struct {
            LuaFunction func;
        };
        struct {
            LuaIndexer getter;
            LuaIndexer setter;
        };
    };
};

/* 类型转换器
 * 基础类型可以子类->基类，基类->子类
 * std::shared_ptr<Ty> 只能子类->基类
*/
struct TypeConverter {
    LuaPointerConvert convert_up;
    LuaPointerConvert convert_down;
    LuaSharedPtrConvert convert_shared_ptr;
};

enum class TypeCategory
{
    kInternal,
    kExternal,
    kGlobal,
};

struct TypeInfo {
    TypeKey id;
    TypeCategory category;
    const char* name;
    bool is_weak_obj;           //
    int external_index;         // 外部类型编号, 用于lightuserdata索引类型
    const TypeInfo* super;
    TypeMember* members;
    TypeMember* globals;
    TypeConverter converter;
};

struct ITypeDesc {
    virtual ~ITypeDesc() { }
    virtual void SetConverter(LuaPointerConvert up, LuaPointerConvert down, LuaSharedPtrConvert shared_ptr) = 0;
    virtual void AddMember(TypeMember member, bool global) = 0;
    virtual TypeKey Finalize() = 0;
};

ITypeDesc* AllocTypeInfo(const char* name, const TypeInfo* super);
const TypeInfo* GetTypeInfo(const TypeKey& key);

class ObjIndex
{
    friend class detail::GlobalVar;
public:
    ObjIndex() : index_(-1) { }
    ~ObjIndex();

public:
    ObjIndex(const ObjIndex&) { }
    ObjIndex& operator = (const ObjIndex&) { }

private:
    int index_;
};

XLUA_NAMESPACE_END

/* 声明导出Lua类 */
#define XLUA_DECLARE_CLASS(ClassName, ...)                              \
    typedef xlua::Declare<ClassName,                                    \
        typename xlua::detail::BaseType<_VAR_ARGS_>::type> LuaDeclare;  \
    xlua::ObjIndex xlua_obj_index_;                                     \
    static const xlua::TypeInfo* xLuaGetTypeInfo()

/* 声明导出外部类 */
#define XLUA_DECLARE_EXTERNAL_CLASS(ClassName)                          \
    const xlua::TypeInfo* xLuaGetTypeInfo(xlua::Identity<ClassName>)
