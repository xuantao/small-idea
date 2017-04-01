/*
 * scoped objects buffer
 * xuantao, 2017
*/

#include <type_traits>
#include "scoped_buffer.h"

#pragma once

namespace zh
{
    /*
    * 用scoped_buffer构造的Obj数组
    */
    template <class Ty>
    class scoped_obj_buffer : public scoped_buffer
    {
    public:
        scoped_obj_buffer(scoped_obj_buffer&& other)
            : scoped_buffer(other)
        {
        }

        scoped_obj_buffer(scoped_buffer&& buffer, size_t count)
            : scoped_buffer(buffer)
        {
            construct(std::conditional<std::is_pod<Ty>::value, std::true_type, std::false_type>::type());
        }

        template <typename ...Args>
        scoped_obj_buffer(scoped_buffer&& buffer, size_t count, Args&& ...args)
            : scoped_buffer(buffer)
        {
            construct(args...);
        }

        ~scoped_obj_buffer()
        {
            destruct(std::conditional< std::is_pod<Ty>::value, std::true_type, std::false_type>::type());
        }

    public:
        Ty* get() const { return (Ty*)scoped_buffer::get(); }
        size_t count() const { return size() / sizeof(Ty); }

    protected:
        scoped_obj_buffer(const scoped_obj_buffer&);
        scoped_obj_buffer& operator = (const scoped_obj_buffer&);

    protected:
        void construct(std::false_type)
        {
            for (size_t i = 0; i < count(); ++i)
                new (&get()[i]) Ty();
        }

        template <typename ...Args>
        void construct(Args&& ...args)
        {
            for (size_t i = 0; i < count(); ++i)
                new (&get()[i]) Ty(args...);
        }

        void destruct(std::false_type)
        {
            size_t idx = count();
            while (idx)
                (&get()[--idx])->~Ty();
        }

        void construct(std::true_type) {}
        void destruct(std::true_type) {}

        //std::conditional<
        //    std::is_pod<Ty>::value,
        //    std::true_type,
        //    std::false_type>::type choose() { return {}; }
    };
}
