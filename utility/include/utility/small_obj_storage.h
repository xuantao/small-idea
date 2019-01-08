/*
 * 小对象缓存器
*/
#pragma once
#include "common.h"
#include <type_traits>

UTILITY_NAMESPACE_BEGIN

/* 小对象缓存池大小配置 */
constexpr int SmallObjectNumPtrs = 6 + 16 / sizeof(void*);
constexpr size_t SpaceSize = (SmallObjectNumPtrs - 1) * sizeof(void*);

template <typename Ty>
struct IsLarge : std::bool_constant<SpaceSize < sizeof(Ty)> {};

template <typename Ty>
class SmallObjStorage
{
public:
    SmallObjStorage() { Set(nullptr); }
    ~SmallObjStorage() { Release(); }

    /* not support copy, unknown how to copy */
    SmallObjStorage(const SmallObjStorage&) = delete;
    SmallObjStorage& operator = (const SmallObjStorage&) = delete;

public:
    inline bool IsEmpty() const { return Get() == nullptr; }
    inline bool IsLocal() const { return GetSpace() == Get(); }
    inline Ty* Get() const { return (real_.ptrs_[SmallObjectNumPtrs - 1]); }
    inline void Set(Ty* ptr) { real_.ptrs_[SmallObjectNumPtrs - 1] = ptr; }
    inline void* GetSpace() { return (&real_); }
    inline const void *GetSpace() const { return (&real_); }

    template <typename Impl, typename... Args>
    void Construct(Args&&... args)
    {
        Release();
        DoConstruct<Impl>(IsLarge<Impl>(), std::forward<Args>(args)...);
    }

    void Release()
    {
        if (IsEmpty())
            return;

        if (IsLocal())
            Get()->~Ty();
        else
            delete Get();
        Set(nullptr);
    }

private:
    template <typename Impl, typename... Args>
    inline void DoConstruct(std::true_type, Args&&... args)
    {
        Set(new Impl(std::forward<Args>(args)...));
    }

    template <typename Impl, typename... Args>
    inline void DoConstruct(std::false_type, Args&&... args)
    {
        Set(new (GetSpace()) Impl(std::forward<Args>(args)...));
    }

private:
    union
    {
        std::max_align_t dummy_1_;
        char dummy_2_[SpaceSize];
        Ty* ptrs_[SmallObjectNumPtrs];
    } real_;
};

UTILITY_NAMESPACE_END
