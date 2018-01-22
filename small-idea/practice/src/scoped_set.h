﻿/*
 * scoped set
 * xuantao, 2017
*/
#pragma once

#include "detail/_tree.h"
#include <xstddef>

NAMESPACE_BEGIN

namespace detail
{
    template <class Ty, class Pr>
    class _set_traits
    {
    public:
        typedef Ty key_type;
        typedef Ty value_type;
        typedef Pr key_compare;
        typedef key_compare value_compare;

        static const key_type& Kfn(const value_type& val) { return val; }
    };
}

/*
 * 域范围内有效的set容器
 * 1. 容器的大小固定不可更改, 需要在初始化是确定
 * 2. 容器不可以被复制和持有
*/
template <class Ty, class Pr = std::less<Ty> >
class scoped_set : public detail::_tree<detail::_set_traits<Ty, Pr> >
{
public:
    typedef scoped_set<Ty, Pr> _my_type;
    typedef detail::_tree<detail::_set_traits<Ty, Pr> > _my_base;
    typedef typename _my_base::size_type size_type;
    typedef typename _my_base::difference_type difference_type;
    typedef typename _my_base::pointer pointer;
    typedef typename _my_base::const_pointer const_pointer;
    typedef typename _my_base::reference reference;
    typedef typename _my_base::const_reference const_reference;
    typedef typename _my_base::iterator iterator;
    typedef typename _my_base::const_iterator const_iterator;

public:
    scoped_set(scoped_buffer&& buffer) : _my_base(std::forward<scoped_buffer>(buffer))
    {
    }

    scoped_set(_my_type&& other) : _my_base(std::forward<_my_type>(other))
    {
    }
};

NAMESPACE_END
