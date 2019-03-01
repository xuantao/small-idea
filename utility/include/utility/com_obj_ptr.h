/*
 * object auto release ptr holder
*/
#pragma once
#include <memory>

namespace com_ptr_internal
{
    template <typename Ty>
    struct HasAddRef
    {
        template<typename U> static auto check(int) -> decltype(std::declval<U*>()->AddRef(), std::true_type());
        template<typename U> static std::false_type check(...);

        static constexpr bool value = decltype(check<Ty>(0))::value;
    };

    template <typename Ty>
    struct HasRelease
    {
        template<typename U> static auto check(int) -> decltype(std::declval<U*>()->Release(), std::true_type());
        template<typename U> static std::false_type check(...);

        static constexpr bool value = decltype(check<Ty>(0))::value;
    };
}

template <typename Ty>
class ComObjPtrBase
{
public:
    ComObjPtrBase() : obj_(nullptr) { }
    ComObjPtrBase(Ty* ptr) : obj_(ptr) { }

    template <typename Oth>
    ComObjPtrBase(ComObjPtrBase<Oth>&& other) : obj_(other.Detach()) { }

    ~ComObjPtrBase() { Release(); }

    ComObjPtrBase(const ComObjPtrBase& other) = delete;
    ComObjPtrBase& operator = (const ComObjPtrBase& other) = delete;

public:
    inline void Release()
    {
        if (obj_)
        {
            obj_->Release();
            obj_ = nullptr;
        }
    }

    inline Ty* Detach()
    {
        auto tmp = obj_;
        obj_ = nullptr;
        return tmp;
    }

    inline Ty* Get() const { return obj_; }
    inline Ty* operator -> () const { return Get(); }
    inline explicit operator bool() const { return Get() != nullptr; }

protected:
    template <typename Oth>
    inline void Set(Oth* obj)
    {
        Release();
        obj_ = obj;
    }

private:
    Ty* obj_;
};

template <typename Ly, typename Ry> inline bool operator == (const ComObjPtrBase<Ly>& l, const ComObjPtrBase<Ry>& r) { return l.Get() == r.Get(); }
template <typename Ly, typename Ry> inline bool operator != (const ComObjPtrBase<Ly>& l, const ComObjPtrBase<Ry>& r) { return !(l == r); }
template <typename Ty> inline bool operator == (const ComObjPtrBase<Ty>& l, std::nullptr_t) { return l.Get() == nullptr; }
template <typename Ty> inline bool operator != (const ComObjPtrBase<Ty>& l, std::nullptr_t) { return l.Get() != nullptr; }
template <typename Ty> inline bool operator == (std::nullptr_t, const ComObjPtrBase<Ty>& l) { return l.Get() == nullptr; }
template <typename Ty> inline bool operator != (std::nullptr_t, const ComObjPtrBase<Ty>& l) { return l.Get() != nullptr; }

/*
 * 持有并管理COM对象指针
*/
template <typename Ty>
class ComObjPtr : public ComObjPtrBase<Ty>
{
    using BaseType = ComObjPtrBase<Ty>;
    static_assert(com_ptr_internal::HasAddRef<Ty>::value && com_ptr_internal::HasRelease<Ty>::value,
        "type need has AddRef & Release interface");
public:
    ComObjPtr() : BaseType() { }
    ComObjPtr(Ty* ptr) : BaseType(ptr) { }
    ComObjPtr(const ComObjPtr& other) { this->operator = (other); }

    template <typename Oth>
    ComObjPtr(const ComObjPtr<Oth>& other) { this->operator = (other); }
    template <typename Oth>
    ComObjPtr(ComObjPtr<Oth>&& other) : BaseType(std::move(other)) { }

    ~ComObjPtr() { }

public:
    inline ComObjPtr& operator = (const ComObjPtr& other)
    {
        this->Attach(other.Get());
        return *this;
    }

    template <typename Oth>
    inline ComObjPtr& operator = (const ComObjPtr<Oth>& other)
    {
        this->Attach(other.Get());
        return *this;
    }

    template <typename Oth>
    inline ComObjPtr& operator = (ComObjPtr<Oth>&& other)
    {
        this->Set(other.Detach());
        return *this;
    }

    template <typename Oth>
    inline ComObjPtr& operator = (Oth* obj)
    {
        this->Set(obj);
        return *this;
    }

private:
    template <typename Oth>
    inline void Attach(Oth* obj)
    {
        this->Set(obj);
        if (obj)
            obj->AddRef();
    }
};

/*
 * 唯一Com对象指针
*/
template <typename Ty>
class UniqueObjPtr : public ComObjPtrBase<Ty>
{
    using BaseType = ComObjPtrBase<Ty>;
    static_assert(com_ptr_internal::HasRelease<Ty>::value, "type need has AddRef & Release interface");
public:
    UniqueObjPtr() : BaseType() { }
    UniqueObjPtr(Ty* ptr) : BaseType(ptr) { }

    template <typename Oth>
    UniqueObjPtr(UniqueObjPtr<Oth>&& other) : BaseType(std::move(other)) { }

    ~UniqueObjPtr() { }

public:
    template <typename Oth>
    inline void operator = (UniqueObjPtr<Oth>&& other)
    {
        this->Set(other.Detach());
    }

    template <typename Oth>
    inline void operator = (Oth* obj)
    {
        this->Set(obj);
    }
};
