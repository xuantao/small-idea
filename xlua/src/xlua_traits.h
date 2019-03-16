#pragma once
#include "xlua_def.h"
#include <type_traits>

XLUA_NAMESPACE_BEGIN

namespace detail {
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

    template <typename Ty>
    struct IsExtendLoad {
        template <typename U> static auto Check(int)->decltype(::xLuaLoad(std::declval<xLuaState*>(), (int)0, Identity<Ty>()), std::true_type());
        template <typename U> static auto Check(...)->std::false_type;

        static constexpr bool value = decltype(Check<Ty>(1))::value;
    };

    template <typename Ty>
    struct IsExtendPush {
        template <typename U> static auto Check(int)->decltype(::xLuaPush(std::declval<xLuaState*>(), std::declval<const Ty&>()), std::true_type());
        template <typename U> static auto Check(...)->std::false_type;

        static constexpr bool value = decltype(Check<Ty>(1))::value;
    };

    struct tag_unknown {};
    struct tag_internal {};
    struct tag_external {};
    struct tag_extend {};

    template <typename Ty>
    struct Dispatcher {
        typedef typename std::conditional<IsInternal<Ty>::value, tag_internal,
            typename std::conditional<IsExternal<Ty>::value, tag_external, tag_unknown>::type>::type type;
    };

    template <typename Ty>
    auto GetTypeInfo() -> typename std::enable_if<IsInternal<Ty>::value, const TypeInfo*>::type {
        return Ty::xLuaGetTypeInfo();
    }

    template <typename Ty>
    auto GetTypeInfo() -> typename std::enable_if<IsExternal<Ty>::value, const TypeInfo*>::type {
        return xLuaGetTypeInfo(Identity<Ty>());
    }

    template <typename Ty>
    auto GetTypeInfo() -> typename std::enable_if<std::is_void<Ty>::value, const TypeInfo*>::type {
        return nullptr;
    }
} // namespace detail

XLUA_NAMESPACE_END
