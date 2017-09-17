/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once
#include <cstring>
#include "Cfg.h"

namespace Enum
{
    namespace detail
    {
        // declare enum info template type
        template <class Ty> struct Info { };

        template <>
        struct Info<GlobalType>
        {
            static const int size = 5;
            static const char* const names[];
            static const GlobalType vals[];
        };

        template <>
        struct Info<Enum1>
        {
            static const int size = 2;
            static const char* const names[];
            static const Enum1 vals[];
        };

        template <>
        struct Info<Detail::Enum2>
        {
            static const int size = 2;
            static const char* const names[];
            static const Detail::Enum2 vals[];
        };

        template <>
        struct Info<Wtf>
        {
            static const int size = 5;
            static const char* const names[];
            static const Wtf vals[];
        };
    } // namespace detail

    template <class En>
    const char* Name(En val)
    {
        typedef detail::Info<En> Info;

        for (int i = 0; i < Info::size; ++i)
            if (Info::vals[i] == val)
                return Info::names[i];

        return nullptr;
    }

    template <class En>
    const char* Name(int idx)
    {
        typedef detail::Info<En> Info;

        if (idx >= 0 && idx < Info::size)
            return Info::names[idx];
        return nullptr;
    }

    template <class En>
    En Value(const char* name)
    {
        typedef detail::Info<En> Info;

        for (int i = 0; i < Info::size; ++i)
            if (std::strcmp(Info::names[i], name) == 0)
                return Info::vals[i];


        return Info::vals[0];
    }

    template <class En>
    En Value(int idx)
    {
        typedef detail::Info<En> Info;

        if (idx >= 0 && idx < Info::size)
            return Info::vals[idx];
        return Info::vals[0];
    }

    template<class En>
    int Index(En val)
    {
        typedef detail::Info<En> Info;

        for (int i = 0; i < Info::size; ++i)
            if (Info::vals[i] == val)
                return i;

        return -1;
    }

    template <class En>
    int Index(const char* name)
    {
        typedef detail::Info<En> Info;

        for (int i = 0; i < Info::size; ++i)
            if (std::strcmp(Info::names[i], name) == 0)
                return i;

        return -1;
    }
} // namespace Enum
