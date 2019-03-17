/*
 * weak_obj_ptr
 */
#pragma once
#include "common.h"
#include "weak_obj_array.h"
#include <vector>

UTILITY_NAMESPACE_BEGIN

namespace weak_obj_ptr_internal
{
    // some problem(multy inherited)
    class WeakObjPtrBase
    {
    public:
        WeakObjPtrBase() : index_(vals::kInvalidIndex), serial_num_(0) { }
        WeakObjPtrBase(const WeakObjPtrBase& other) = default;
        WeakObjPtrBase& operator = (const WeakObjPtrBase& other) = default;

    public:
        inline bool IsValid() const
        {
            return index_ != vals::kInvalidIndex &&
                serial_num_ == WeakObjArray::GetInstance()->GetSerialNum(index_);
        }

        inline void* Get() const
        {
            if (!IsValid())
                return nullptr;
            return WeakObjArray::GetInstance()->GetObject(index_);
        }

        template <typename Ty>
        inline void Set(Ty* obj)
        {
            index_ = WeakObjArray::GetInstance()->AllocIndex(obj);
            serial_num_ = WeakObjArray::GetInstance()->GetSerialNum(index_);
            assert(index_ != vals::kInvalidIndex);
        }

        inline void Reset()
        {
            index_ = vals::kInvalidIndex;
            serial_num_ = 0;
        }

        inline bool operator == (const WeakObjPtrBase& other) const
        {
            return index_ == other.index_ && serial_num_ == other.serial_num_;
        }

        inline bool operator != (const WeakObjPtrBase& other) const
        {
            return !(this->operator==(other));
        }

    private:
        int index_;
        int serial_num_;
    };
} // namespace weak_obj_ptr_internal

/* 弱指针 */
template <typename Ty>
class WeakObjPtr : private weak_obj_ptr_internal::WeakObjPtrBase
{
    static_assert(HasObjectIndex<Ty>::value, _SUPPORT_WEAK_OBJ_PTR_ERROR);
    using Super = weak_obj_ptr_internal::WeakObjPtrBase;
public:
    WeakObjPtr() = default;
    WeakObjPtr(const WeakObjPtr&) = default;
    WeakObjPtr(std::nullptr_t) { }

    template <typename U>
    WeakObjPtr(U* obj) { this->operator = (obj); }

    template <typename U>
    WeakObjPtr(const WeakObjPtr<U>& other) { this->operator = (other); }

public:
    WeakObjPtr& operator = (const WeakObjPtr&) = default;

    template <typename U>
    inline void operator = (U* obj)
    {
        Ty* tmp = obj;
        Super::Set(tmp);
    }

    template <typename U>
    inline void operator = (const WeakObjPtr<U>& Other)
    {
        static_assert(std::is_convertible<U*, Ty*>::value, "Unable to convert WeakObjPtr - types are incompatible");
        *(Super*)this = *(Super*)&Other;
    }

public:
    inline Ty* Get() const { return static_cast<Ty*>(Super::Get()); }
    inline void Reset() { Super::Reset(); }
    inline bool IsValid() const { return Super::IsValid(); }
    inline operator bool() const { return IsValid(); }
};

template <typename Ty> inline bool operator == (const WeakObjPtr<Ty>& ptr, std::nullptr_t) { return !ptr.IsValid(); }
template <typename Ty> inline bool operator != (const WeakObjPtr<Ty>& ptr, std::nullptr_t) { return ptr.IsValid(); }
template <typename Ty> inline bool operator == (std::nullptr_t, const WeakObjPtr<Ty>& ptr) { return !ptr.IsValid(); }
template <typename Ty> inline bool operator != (std::nullptr_t, const WeakObjPtr<Ty>& ptr) { return ptr.IsValid(); }

template <typename Ly, typename Ry>
inline bool operator == (const WeakObjPtr<Ly>& l, const WeakObjPtr<Ry>& r)
{
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr - types are incompatible");
    return (const weak_obj_ptr_internal::WeakObjPtrBase&)l == (const weak_obj_ptr_internal::WeakObjPtrBase&)r;
}

template <typename Ly, typename Ry>
inline bool operator != (const WeakObjPtr<Ly>& l, const WeakObjPtr<Ry>& r)
{
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr - types are incompatible");
    return (const weak_obj_ptr_internal::WeakObjPtrBase&)l != (const weak_obj_ptr_internal::WeakObjPtrBase&)r;
}

template <typename Ly, typename Ry>
inline bool operator == (const WeakObjPtr<Ly>& l, const Ry* r)
{
    static_assert(HasObjectIndex<Ry>::value, _SUPPORT_WEAK_OBJ_PTR_ERROR);
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr with raw pointer - types are incompatible");

    return l.Get() == static_cast<const Ly*>(r);
}

template <typename Ly, typename Ry>
inline bool operator != (const WeakObjPtr<Ly>& l, const Ry* r)
{
    static_assert(HasObjectIndex<Ry>::value, _SUPPORT_WEAK_OBJ_PTR_ERROR);
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr with raw pointer - types are incompatible");

    return l.Get() != static_cast<const Ly*>(r);
}

template <typename Ly, typename Ry>
inline bool operator == (const Ly* l, const WeakObjPtr<Ry>& r)
{
    static_assert(HasObjectIndex<Ly>::value, _SUPPORT_WEAK_OBJ_PTR_ERROR);
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr with raw pointer - types are incompatible");
    return static_cast<const Ry*>(l) == r.Get();
}

template <typename Ly, typename Ry>
inline bool operator != (const Ly* l, const WeakObjPtr<Ry>& r)
{
    static_assert(HasObjectIndex<Ly>::value, _SUPPORT_WEAK_OBJ_PTR_ERROR);
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr with raw pointer - types are incompatible");
    return static_cast<const Ry*>(l) != r.Get();
}

UTILITY_NAMESPACE_END
