/*
 * tree
*/
#pragma once

#include "def.h"

NAMESPACE_ZH_BEGIN

namespace detail
{
    template <class Ty>
    struct _tree_node
    {
        _tree_node* left;
        _tree_node* right;
        _tree_node* parent;
        char color;
        bool isNil;
        Ty val;
    };

    template <class Traits>
    class _tree
    {

    };
}

NAMESPACE_ZH_END
