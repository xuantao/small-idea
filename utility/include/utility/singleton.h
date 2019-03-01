/*
 * singleton base
*/
#pragma once
#include "common.h"

UTILITY_NAMESPACE_BEGIN

template <typename Ty>
class Singleton
{
protected:
    Singleton()
    {
        assert(instnace_ == nullptr);
        instnace_ = static_cast<Ty*>(this);
    }

    ~Singleton() { instnace_ = nullptr; }

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator = (const Singleton&) = delete;

public:
    static inline Ty* GetInstance() { return instnace_; }

private:
    static Ty* instnace_;
};

template <typename Ty>
Ty* Singleton<Ty>::instnace_ = nullptr;

UTILITY_NAMESPACE_END
