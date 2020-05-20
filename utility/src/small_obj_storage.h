/*
 * 小对象缓存器
*/
#pragma once
#include "common.h"
#include <type_traits>

UTILITY_NAMESPACE_BEGIN

template <typename Ty>
struct IsLarge : std::integral_constant<bool, vals::kSmallObjectSpaceSize < sizeof(Ty)> {};

template <typename Ty>
class SmallObjStorage
{
    static_assert(!std::is_array<Ty>::value, "small object storage not support array");

public:
    SmallObjStorage() { Set(nullptr); }
    ~SmallObjStorage() { Release(); }

    /* not support copy, unknown how to copy */
    SmallObjStorage(const SmallObjStorage&) = delete;
    SmallObjStorage& operator = (const SmallObjStorage&) = delete;

public:
    inline bool IsEmpty() const { return Get() == nullptr; }
    inline bool IsLocal() const { return GetSpace() == Get(); }
    inline Ty* Get() const { return (ptrs_[vals::kSmallObjectNumPtrs - 1]); }
    inline void Set(Ty* ptr) { ptrs_[vals::kSmallObjectNumPtrs - 1] = ptr; }
    inline void* GetSpace() { return (ptrs_); }
    inline const void* GetSpace() const { return (ptrs_); }

    template <typename Impl, typename... Args>
    void Construct(Args&&... args)
    {
        static_assert(std::is_convertible<Impl*, Ty*>::value, "construct object should be convertible to target");

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
        std_ext::max_align_t dummy_1_;
        char dummy_2_[vals::kSmallObjectSpaceSize];
        Ty* ptrs_[vals::kSmallObjectNumPtrs];
    };
};


template <typename Ty>
struct ObjStorage {
    ObjStorage() = default;
    ObjStorage(const ObjStorage& other) { *this = other; }
    ObjStorage(ObjStorage&& other) { *this = std::move(other); }

    template <typename... Args>
    ObjStorage(Args&&... args) {
        obj_ = new (space_) Ty(std::forward<Args>(args)...);
    }

    ~ObjStorage() { Release(); }

    inline ObjStorage& operator=(const ObjStorage& other) {
        Release();
        if (other)
            Construct(*other.Get());
        return *this;
    }

    inline ObjStorage& operator=(ObjStorage&& other) {
        Release();
        if (other) {
            Construct(std::move(*other.Get()));
            other.Release();
        }
        return *this;
    }

public:
    inline explicit operator bool() const { return obj_ != nullptr; }
    inline Ty& operator*() const { return *Get(); }
    inline Ty* Get() const { return obj_; }

    template <typename... Args>
    inline void Construct(Args&&... args) {
        assert(obj_ == nullptr);
        obj_ = new (space_) Ty(std::forward<Args>(args)...);
    }

    inline void Release() {
        if (obj_) {
            obj_->~Ty();
            obj_ = nullptr;
        }
    }

private:
    Ty* obj_ = nullptr;
    char space_[sizeof(Ty)];
};

UTILITY_NAMESPACE_END
