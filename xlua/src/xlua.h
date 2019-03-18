#pragma once
#include "xlua_def.h"
#include "detail/traits.h"
#include <lua.hpp>
#include <string>
#include <unordered_map>

/* 扩展类型
 * void xLuaPush(xlua::xLuaState* l, const Type& val)
 * Type xLuaLoad(xlua::xLuaState* L, int index, xlua::Identity<Type>);
 * const char* xLuaTypeName(xlua::Identity<Type>);
*/

XLUA_NAMESPACE_BEGIN

bool Startup();
void Shutdown();

class xLuaTable { };
class xLuaFunction { };

class xLuaState {
    friend class detail::GlobalVar;
    xLuaState(lua_State* l, bool attach);
    ~xLuaState();

public:
    inline bool IsAttach() const { return attach_; }
    inline lua_State* GetState() const { return state_; }

public:
    template <typename Ty>
    inline void Push(Ty&& val) {
        DoPush(val);
    }

    template <typename Ty>
    inline Ty Load(int index) {
        using tag = typename std::conditional<std::is_pointer<Ty>::value, std::true_type, std::false_type>::type;
        return DoLoad<Ty>(index, tag());
    }

    inline void Push(bool val) { }
    inline void Push(char val) { }
    inline void Push(unsigned char val) { }
    inline void Push(short val) { }
    inline void Push(unsigned short val) { }
    inline void Push(int val) { }
    inline void Push(unsigned int val) { }
    inline void Push(long val) { }
    inline void Push(unsigned long val) { }
    inline void Push(long long val) { }
    inline void Push(unsigned long long val) { }
    inline void Push(float val) { }
    inline void Push(double val) { }
    inline void Push(char* val) { }
    inline void Push(const char* val) { }
    inline void Push(const std::string& val) { }
    //void Push(const xLuaTable& val) { }
    //void Push(const xLuaFunction& val) { }

    template<> inline bool Load<bool>(int index) { return false; }
    template<> inline char Load<char>(int index) { return false; }
    template<> inline unsigned char Load<unsigned char>(int index) { return false; }
    template<> inline short Load<short>(int index) { return false; }
    template<> inline unsigned short Load<unsigned short>(int index) { return false; }
    template<> inline int Load<int>(int index) { return false; }
    template<> inline unsigned int Load<unsigned int>(int index) { return false; }
    template<> inline long Load<long>(int index) { return false; }
    template<> inline unsigned long Load<unsigned long>(int index) { return false; }
    template<> inline long long Load<long long>(int index) { return false; }
    template<> inline unsigned long long Load<unsigned long long>(int index) { return false; }
    template<> inline float Load<float>(int index) { return false; }
    template<> inline double Load<double>(int index) { return false; }
    template<> inline const char* Load<const char*>(int index) { return false; }
    template<> inline std::string Load<std::string>(int index) { return false; }
    template<> inline const std::string Load<const std::string>(int index) { return false; }

private:
    template <typename Ty>
    inline void DoPush(const Ty& val) {
        using raw_ty = typename std::decay<Ty>::type;
        static_assert(!std::is_pointer<raw_ty>::value, "not allow pointer to pointer");
        static_assert(!detail::IsInternal<raw_ty>::value, "not allow push internal class value");
        static_assert(detail::IsExternal<raw_ty>::value || detail::IsExtendPush<raw_ty>::value,
            "only declare external or extend push type value can push to lua");
        using tag = typename std::conditional<detail::IsExternal<raw_ty>::value,
            detail::tag_external, detail::tag_extend>::type;
        PushValue(val, tag());
    }

    template <typename Ty>
    inline void DoPush(Ty* val) {
        using raw_ty = typename std::decay<Ty>::type;
        static_assert(!std::is_pointer<raw_ty>::value, "not allow pointer to pointer");
        static_assert(!detail::IsExtendPush<raw_ty>::value, "can not push extend pointer to lua");
        static_assert(detail::IsInternal<raw_ty>::value || detail::IsExternal<raw_ty>::value,
            "only declare internal or external type pointer can push to lua");

        if (val == nullptr) {
            Push(nullptr);
            return;
        }

        using tag = typename std::conditional<detail::IsWeakObjPtr<raw_ty>::value, detail::tag_weakobj,
            typename std::conditional<detail::IsInternal<raw_ty>::value, detail::tag_internal, detail::tag_external>::type>::type;
        PushPointer(val, tag());
    }

    template <typename Ty>
    inline void PushValue(const Ty& val, detail::tag_external) {
        printf("push external value\n");
    }

    template <typename Ty>
    inline void PushValue(const Ty& val, detail::tag_extend) {
        printf("push extend value\n");
        ::xLuaPush(this, val);
    }

    template <typename Ty>
    inline void PushPointer(Ty* val, detail::tag_weakobj) {
        printf("push weak obj ptr\n");
    }

    template <typename Ty>
    inline void PushPointer(Ty* val, detail::tag_internal) {
        printf("push internal ptr\n");
    }

    template <typename Ty>
    inline void PushPointer(Ty* val, detail::tag_external) {
        printf("push external ptr\n");
    }

    template <typename Ty>
    Ty DoLoad(int index, std::true_type) {

    }

    template <typename Ty>
    Ty DoLoad(int index, std::false_type) {

    }

private:
    bool attach_;
    lua_State* state_;
    int meta_table_index_;
    int lua_obj_table_index_;
    int user_data_table_index_;
    std::unordered_map<void*, int> user_datas_;
};

namespace detail {
    template <typename Ty>
    inline void PushValue(xLuaState* l, const Ty& val, tag_external) {
        printf("push external value\n");
    }

    template <typename Ty>
    inline void PushValue(xLuaState* l, const Ty& val, tag_extend) {
        printf("push extend value\n");
        ::xLuaPush(l, val);
    }

    template <typename Ty>
    inline void PushPointer(xLuaState* l, Ty* val, tag_internal) {
        printf("push internal ptr\n");
    }

    template <typename Ty>
    inline void PushPointer(xLuaState* l, Ty* val, tag_external) {
        printf("push external ptr\n");
    }

    template <typename Ty>
    inline void PushPointer(xLuaState* l, Ty* val, tag_weakobj) {
        printf("push weak obj ptr\n");
    }

    template <typename Ty>
    inline void DoPush(xLuaState* l, const Ty& val) {
        using raw_ty = typename std::decay<Ty>::type;
        static_assert(!std::is_pointer<raw_ty>::value, "not allow pointer to pointer");
        static_assert(!IsInternal<raw_ty>::value, "not allow push internal class value");
        static_assert(IsExternal<raw_ty>::value || IsExtendPush<raw_ty>::value,
            "only declare external or extend push type value can push to lua");
        using tag = typename std::conditional<IsExternal<raw_ty>::value,
            tag_external, tag_extend>::type;
        PushValue(l, val, tag());
    }

    template <typename Ty>
    inline void DoPush(xLuaState* l, Ty* val) {
        using raw_ty = typename std::decay<Ty>::type;
        static_assert(!std::is_pointer<raw_ty>::value, "not allow pointer to pointer");
        static_assert(!IsExtendPush<raw_ty>::value, "can not push extend pointer to lua");
        static_assert(IsInternal<raw_ty>::value || IsExternal<raw_ty>::value,
            "only declare internal or external type pointer can push to lua");

        if (val == nullptr) {
            l->Push(nullptr);
            return;
        }

        using tag = typename std::conditional<IsWeakObjPtr<raw_ty>::value, tag_weakobj,
            typename std::conditional<IsInternal<raw_ty>::value, tag_internal, tag_external>::type>::type;
        PushPointer(l, val, tag());
    }
} // namespace detail

XLUA_NAMESPACE_END
