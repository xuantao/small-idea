#pragma once
#include <lua.hpp>
#include <type_traits>
#include <cstdint>
#include <cassert>

#define XLUA_NAMESPACE_BEGIN    namespace xlua {
#define XLUA_NAMESPACE_END      } // namespace xlua

/* 64位系统开启LIGGHT_USER_DATA
 * 导出对象指针使用LightUserData代替FullUserData
*/
#if INTPTR_MAX == INT64_MAX
#ifndef XLUA_USE_LIGHT_USER_DATA
    #define XLUA_USE_LIGHT_USER_DATA 1
#endif
#endif

/* 配置是否支持弱指针
 * 修改基类定义宏并实例化基础接口
*/
#ifndef XLUA_WEAK_OBJ_BASE_TYPE
    #define XLUA_WEAK_OBJ_BASE_TYPE void
    template <typename Ty> struct xLuaWeakObjPtr {};
    inline int xLuaAllocWeakObjIndex(void* val) { assert(false); return -1; }
    inline int xLuaGetWeakObjSerialNum(int index) { assert(false); return 0; }
    inline void* xLuaGetWeakObjPtr(int index) { assert(false); return nullptr; }
#else // XLUA_WEAK_OBJ_BASE_TYPE
    template <typename Ty> struct xLuaWeakObjPtr;
    int xLuaAllocWeakObjIndex(XLUA_WEAK_OBJ_BASE_TYPE* val);
    int xLuaGetWeakObjSerialNum(int index);
    XLUA_WEAK_OBJ_BASE_TYPE* xLuaGetWeakObjPtr(int index);
#endif // XLUA_WEAK_OBJ_BASE_TYPE


#define XLUA_MAX_TYPE_NAME_LENGTH   64
#define XLUA_MAX_MEMBER_NAME_LENGTH 64

XLUA_NAMESPACE_BEGIN

namespace detail {
    class GlobalVar;

    template <typename... Tys> struct BaseType { static_assert(sizeof...(Tys) > 1, "not allow multy inherit"); };
    template <> struct BaseType<> { typedef void type; };
    template <typename Ty> struct BaseType<Ty> { typedef Ty type; };
}

class xLuaState;
struct TypeInfo;

/* 导出到Lua类型 */
typedef int (*LuaFunction)(lua_State* L);
typedef void (*LuaIndexer)(xLuaState* L, void* obj);

/* convert to weak_obj_ptr */
typedef void* (*ToWeakPtr)(void* obj);
typedef void* (*PtrCast)(void* obj, const TypeInfo* src, const TypeInfo* dst);

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
*/
struct TypeCaster {
    PtrCast to_super;       // 转为基类指针, static_cast
    PtrCast to_derived;     // 转为派生类指针, dynamic_cast
    ToWeakPtr to_weak_ptr;  // 弱对象指针
};

enum class TypeCategory
{
    kInternal,
    kExternal,
    kGlobal,
};

struct TypeInfo {
    int index;
    TypeCategory category;
    const char* type_name;
    const char* shared_ptr_name;
    const char* unique_ptr_name;
    bool is_weak_obj;
    unsigned char external_type_index;  // 外部类型编号, 用于lightuserdata索引类型
    const TypeInfo* super;
    TypeCaster caster;
    TypeMember* members;
    TypeMember* globals;
};

struct ITypeDesc {
    virtual ~ITypeDesc() { }
    virtual void SetCaster(TypeCaster caster) = 0;
    virtual void AddMember(const char* name, LuaFunction func, bool glboal) = 0;
    virtual void AddMember(const char* name, LuaIndexer getter, LuaIndexer setter, bool glboal) = 0;
    virtual const TypeInfo* Finalize() = 0;
};

ITypeDesc* AllocTypeInfo(TypeCategory category, bool is_weak_obj, const char* name, const TypeInfo* super);

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
