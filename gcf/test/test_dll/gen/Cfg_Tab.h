/*
 * this file is auto generated.
 * please does not edit it manual!
*/
#pragma once

#include "Cfg.h"

namespace tab
{
    namespace detail
    {
        // declare Tab Info template type
        template <class Ty> struct Info { };

        template <>
        struct Info<Struct2>
        {
            static const char* const titles[];
            static const char* const types[];
            static const char* const descs[];
        };

        template <>
        struct Info<Sr3>
        {
            static const char* const titles[];
            static const char* const types[];
            static const char* const descs[];
        };
    } // namespace detail

    template <class Ty>
    const char* const * Titles()
    {
        return detail::Info<Ty>::titles;
    }

    template <class Ty>
    const char* const * Types()
    {
        return detail::Info<Ty>::types;
    }

    template <class Ty>
    const char* const * Descs()
    {
        return detail::Info<Ty>::descs;
    }
} // namespace tab
