#pragma once
#include "../xlua_def.h"
#include <type_traits>

XLUA_NAMESPACE_BEGIN

namespace detail {
    template<typename Ty>
    struct IsInternal {
        template <typename U> static auto Check(int)->decltype(std::declval<U>().xLuaGetTypeInfo());
        template <typename U> static auto Check(...)->std::false_type;

        static constexpr bool value = std::is_same<decltype(Check<Ty>(0)), ObjIndex>::value;
    };

    template <typename Ty>
    struct IsExternal {
        template <typename U> static auto Check(int)->decltype(::xLuaGetTypeInfo(Identity<U>()), std::true_type());
        template <typename U> static auto Check(...)->std::false_type;

        static constexpr bool value = decltype(Check<Ty>(0))::value;
    };

    template <typename Ty>
    struct IsExtendLoad {
        template <typename U> static auto Check(int)->decltype(::xLuaLoad(std::declval<xLuaState*>(), (int)0, Identity<Ty>()), std::true_type());
        template <typename U> static auto Check(...)->std::false_type;

        static constexpr bool value = decltype(Check<Ty>(0))::value;
    };

    template <typename Ty>
    struct IsExtendPush {
        template <typename U> static auto Check(int)->decltype(::xLuaPush(std::declval<xLuaState*>(), std::declval<const Ty&>()), std::true_type());
        template <typename U> static auto Check(...)->std::false_type;

        static constexpr bool value = decltype(Check<Ty>(0))::value;
    };

    template <typename Ty>
    struct IsExtendName {
        template <typename U> static auto Check(int)->decltype(::xLuaName(Identity<Ty>()), std::true_type());
        template <typename U> static auto Check(...)->std::false_type;

        static constexpr bool value = decltype(Check<Ty>(0))::value;
    };

    template <typename Ty>
    struct IsWeakObjPtr {
        static constexpr bool value = false;
    };

    struct tag_unknown {};
    struct tag_internal {};
    struct tag_external {};
    struct tag_extend {};
    struct tag_weakobj {};

    //template <typename Ty>
    //struct Dispatcher {
    //    typedef typename std::conditional<IsInternal<Ty>::value, tag_internal,
    //        typename std::conditional<IsExternal<Ty>::value, tag_external, 
    //            typename std::conditional<IsExtendLoad<Ty>::value || IsExtendPush<Ty>::value || IsExtendName<Ty>::value, tag_extend, tag_unknown>::type>::type type;
    //};

    template <typename Ty>
    auto GetTypeInfoImpl() -> typename std::enable_if<IsInternal<Ty>::value, const TypeInfo*>::type {
        return Ty::xLuaGetTypeInfo();
    }

    template <typename Ty>
    auto GetTypeInfoImpl() -> typename std::enable_if<IsExternal<Ty>::value, const TypeInfo*>::type {
        return xLuaGetTypeInfo(Identity<Ty>());
    }

    template <typename Ty>
    auto GetTypeInfoImpl() -> typename std::enable_if<std::is_void<Ty>::value, const TypeInfo*>::type {
        return nullptr;
    }
} // namespace detail

XLUA_NAMESPACE_END
