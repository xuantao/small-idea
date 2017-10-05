#pragma once

namespace tab
{
    namespace detail
    {
        template <typename Ty>
        struct Info;
    }

    template <class Ty>
    class Loader
    {
    public:
        virtual bool Load(Ty& val) = 0;
    };
}
