/*
 * scoped map
 * xuantao, 2017
*/
#pragma once

#include "detail/_tree.h"

NAMESPACE_ZH_BEGIN

namespace detail
{
    template <class Kty, class Ty, class Pr>
    class _map_traits
    {
    public:
        typedef Kty key_type;
        typedef std::pair<const Kty, Ty> value_type;
        typedef Pr key_compare;

        class value_compare
        {
            friend class _map_traits<Kty, Ty, Pr>;
        public:
            bool operator()(const value_type& _Left, const value_type& _Right) const
            {
                return (_cmp(_Left.first, _Right.first));
            }

            value_compare(key_compare _Pred) : _cmp(_Pred)
            {
            }

        protected:
            key_compare _cmp;
        };

        template<class Ty1, class Ty2>
        static const Kty& Kfn(const std::pair<Ty1, Ty2>& val) { return (val.first); }
    };
}

/*
 * 域范围内有效的map容器
 * 1. 容器的大小固定不可更改, 需要在初始化是确定
 * 2. 容器不可以被复制和持有
*/
template <class Kty, class Ty, class Pr = std::less<Kty> >
class scoped_map : public detail::_tree<detail::_map_traits<Kty, Ty, Pr> >
{
public:
    typedef scoped_map<Kty, Ty, Pr> _my_type;
    typedef detail::_tree <detail::_map_traits<Kty, Ty, Pr> > _my_base;
    typedef Ty map_value;
    typedef typename _my_base::key_type key_type;
    typedef typename _my_base::value_type value_type;
    typedef typename _my_base::size_type size_type;
    typedef typename _my_base::difference_type difference_type;
    typedef typename _my_base::pointer pointer;
    typedef typename _my_base::const_pointer const_pointer;
    typedef typename _my_base::reference reference;
    typedef typename _my_base::const_reference const_reference;
    typedef typename _my_base::iterator iterator;
    typedef typename _my_base::const_iterator const_iterator;

public:
    scoped_map(scoped_buffer&& buffer) : _my_base(std::forward<scoped_buffer>(buffer))
    {
    }

    scoped_map(_my_type&& other) : _my_base(std::forward<_my_type>(other))
    {
    }

public:
    map_value& operator [] (const key_type& key)
    {
        return Try_emplace(key).first->second;
    }

protected:
    template <class _Kty, class..._Ty>
    pairib Try_emplace(_Kty&& key, _Ty&&... val)
    {
        iterator it = iterator(&_val, Lbound(key));
        if (it == end() || _camp(key, Key(it._Node())))
            return pairib(Emplace_Hint(it, std::piecewise_construct,
                std::forward_as_tuple(std::forward<_Kty>(key)),
                std::forward_as_tuple(std::forward<_Ty>(val)...)),
                true);
        return pairib(it, false);
    }
};

NAMESPACE_ZH_END
