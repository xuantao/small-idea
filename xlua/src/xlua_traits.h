#pragma once
#include "xlua_def.h"
#include <type_traits>

XLUA_NAMESPACE_BEGIN

namespace detail{
    template<typename Ty>
    struct IsInternal {
        template <typename U> static auto Check(int)->decltype(std::declval<U>().xlua_obj_index_);
        template <typename U> static auto Check(...)->std::false_type;

        static constexpr bool value = std::is_same<decltype(Check<Ty>(1)), ObjIndex>::value;
    };

    template <typename Ty>
    struct IsExternal {
        template <typename U> static auto Check(int)->decltype(::xLuaGetTypeInfo(Identity<U>()), std::true_type());
        template <typename U> static auto Check(...)->std::false_type;

        static constexpr bool value = decltype(Check<Ty>(1))::value;
    };

    struct tag_unknown {};
    struct tag_internal {};
    struct tag_external {};

    template <typename Ty>
    struct Dispatcher {
        typedef typename std::conditional<IsInternal<Ty>::value, tag_internal,
            typename std::conditional<IsExternal<Ty>::value, tag_external, tag_unknown>::type>::type type;
    };

    template <typename Ty>
    auto DoGetTypeInfo() -> typename std::enable_if<IsInternal<Ty>::value, const TypeInfo*>::type {
        return Ty::xLuaGetTypeInfo();
    }

    template <typename Ty>
    auto DoGetTypeInfo() -> typename std::enable_if<IsExternal<Ty>::value, const TypeInfo*>::type {
        return xLuaGetTypeInfo(Identity<Ty>());
    }
} // namespace detail

XLUA_NAMESPACE_END
