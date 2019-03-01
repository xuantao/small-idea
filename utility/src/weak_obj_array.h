/*
 * weak obj ptr
*/
#pragma once
#include "common.h"
#include "singleton.h"
#include <vector>

#define DECLARE_WEAK_OBJ_INDEX_MEMBER   UTILITY_NAMESPACE WeakObjIndex WEAK_OBJ_INDEX_MEMBER_NAME

UTILITY_NAMESPACE_BEGIN

class WeakObjIndex
{
    friend class WeakObjArray;
public:
    WeakObjIndex() : index_(vals::kInvalidIndex) { }
    ~WeakObjIndex();

    WeakObjIndex(const WeakObjIndex&) = delete;
    WeakObjIndex& operator = (const WeakObjIndex&) = delete;

private:
    int index_;
};

namespace weak_obj_internal
{
    template <typename Ty>
    struct HasIndexMember
    {
        template<typename U> static auto Check(int) -> decltype(std::declval<U>(). WEAK_OBJ_INDEX_MEMBER_NAME);
        template<typename U> static std::false_type Check(...);

        static constexpr bool value = std::is_same<decltype(Check<Ty>(0)), WeakObjIndex>::value;
    };

    template <typename Ty>
    struct HasIndex : std::bool_constant<std::is_base_of<WeakObjIndex, Ty>::value || HasIndexMember<Ty>::value>
    {
    };

    struct ArrayObj
    {
        int serial_num; // 序列号
        int next_index; // 下一个空闲元素
        void* obj;      // 对象指针
    };
}

class WeakObjArray : public Singleton<WeakObjArray>
{
    using ArrayObj = weak_obj_internal::ArrayObj;
private:
    WeakObjArray(int inc)
        : inc_(inc)
        , serial_num_gener_(0)
        , free_index(vals::kInvalidIndex)
    {
    }

    ~WeakObjArray() = default;

public:
    static bool Startup(int inc = 1024)
    {
        assert(inc > 0 && GetInstance() == nullptr);
        new WeakObjArray(inc);
        return true;
    }

    void Purge() { delete this; }

public:
    template <typename Ty>
    int AllocIndex(Ty* obj)
    {
        static_assert(weak_obj_internal::HasIndex<Ty>::value, "do not support weak obj");
        assert(obj);
        if (obj == nullptr)
            return vals::kInvalidIndex;

        auto& weak_index = GetIndex(obj);
        if (weak_index.index_ != vals::kInvalidIndex)
            return weak_index.index_;

        if (free_index == vals::kInvalidIndex)
        {
            int old_size = (int)obj_.size();
            int new_size = old_size + inc_;

            obj_.resize(new_size);
            for (int i = old_size; i < new_size; ++i)
            {
                auto& o = obj_[i];
                o.next_index = i + 1;
                o.serial_num = 0;
                o.obj = nullptr;
            }
            obj_.back().next_index = vals::kInvalidIndex;
            free_index = old_size;
        }

        auto index = free_index;
        auto ary_obj = GetArrayObj(index);
        free_index = ary_obj->next_index;

        ary_obj->obj = obj;
        ary_obj->serial_num = ++serial_num_gener_;
        weak_index.index_ = index;
        return index;
    }

    template <typename Ty>
    inline void FreeIndex(Ty* obj)
    {
        static_assert(weak_obj_internal::HasIndex<Ty>::value, "do not support weak obj");
        FreeIndex(GetIndex(obj));
    }

    inline void FreeIndex(WeakObjIndex& weak_index)
    {
        if (weak_index.index_ == vals::kInvalidIndex)
            return;

        auto ary_obj = GetArrayObj(weak_index.index_);
        ary_obj->obj = nullptr;
        ary_obj->next_index = free_index;
        ary_obj->serial_num = 0;

        free_index = weak_index.index_;
        weak_index.index_ = vals::kInvalidIndex;
    }

    inline void* GetObject(int index)
    {
        auto ary_obj = GetArrayObj(index);
        return ary_obj ? ary_obj->obj : nullptr;
    }

    inline int GetSerialNum(int index) const
    {
        auto ary_obj = GetArrayObj(index);
        return ary_obj ? ary_obj->serial_num : 0;
    }

private:
    inline ArrayObj* GetArrayObj(int index)
    {
        assert(index >= 0 && index < (int)obj_.size());
        if (index >= 0 && index < (int)obj_.size())
            return &obj_[index];
        return nullptr;
    }

    inline const ArrayObj* GetArrayObj(int index) const
    {
        return const_cast<WeakObjArray*>(this)->GetArrayObj(index);
    }

    template <typename Ty>
    inline WeakObjIndex& GetIndex(Ty* obj)
    {
        return GetIndex(std::is_base_of<WeakObjIndex, Ty>(), obj);
    }

    template <typename Ty>
    inline WeakObjIndex& GetIndex(std::true_type, Ty* obj)
    {
        return static_cast<WeakObjIndex&>(*obj);
    }

    template <typename Ty>
    inline WeakObjIndex& GetIndex(std::false_type, Ty* obj)
    {
        return obj->WEAK_OBJ_INDEX_MEMBER_NAME;
    }

private:
    int inc_;
    int serial_num_gener_;
    int free_index;
    std::vector<ArrayObj> obj_;
};

WeakObjIndex::~WeakObjIndex()
{
    WeakObjArray::GetInstance()->FreeIndex(*this);
}

namespace weak_obj_internal
{
    class WeakObjBase
    {
    public:
        WeakObjBase() : index_(vals::kInvalidIndex), serial_num_(0) { }
        WeakObjBase(const WeakObjBase& other) = default;
        WeakObjBase& operator = (const WeakObjBase& other) = default;

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

        inline bool operator == (const WeakObjBase& other) const
        {
            return index_ == other.index_ && serial_num_ == other.serial_num_;
        }

        inline bool operator != (const WeakObjBase& other) const
        {
            return !(this->operator==(other));
        }

    private:
        int index_;
        int serial_num_;
    };
} // namespace weak_obj_internal

template <typename Ty>
class WeakObjPtr : private weak_obj_internal::WeakObjBase
{
    static_assert(weak_obj_internal::HasIndex<Ty>::value, "not support weak obj");
    using Super = weak_obj_internal::WeakObjBase;
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
    inline bool IsValid() const { return Super::IsValid(); }
    inline void Reset() { Super::Reset(); }
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
    return (const weak_obj_internal::WeakObjBase&)l == (const weak_obj_internal::WeakObjBase&)r;
}

template <typename Ly, typename Ry>
inline bool operator != (const WeakObjPtr<Ly>& l, const WeakObjPtr<Ry>& r)
{
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr - types are incompatible");
    return (const weak_obj_internal::WeakObjBase&)l != (const weak_obj_internal::WeakObjBase&)r;
}

template <typename Ly, typename Ry>
inline bool operator == (const WeakObjPtr<Ly>& l, const Ry* r)
{
    static_assert(weak_obj_internal::HasIndex<Ry>::value, "WeakObjPtr can only be compared with types");
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr with raw pointer - types are incompatible");

    return l.Get() == static_cast<const Ly*>(r);
}

template <typename Ly, typename Ry>
inline bool operator != (const WeakObjPtr<Ly>& l, const Ry* r)
{
    static_assert(weak_obj_internal::HasIndex<Ry>::value, "WeakObjPtr can only be compared with types");
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr with raw pointer - types are incompatible");

    return l.Get() != static_cast<const Ly*>(r);
}

template <typename Ly, typename Ry>
inline bool operator == (const Ly* l, const WeakObjPtr<Ry>& r)
{
    static_assert(weak_obj_internal::HasIndex<Ly>::value, "WeakObjPtr can only be compared with types");
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr with raw pointer - types are incompatible");
    return static_cast<const Ry*>(l) == r.Get();
}

template <typename Ly, typename Ry>
inline bool operator != (const Ly* l, const WeakObjPtr<Ry>& r)
{
    static_assert(weak_obj_internal::HasIndex<Ly>::value, "WeakObjPtr can only be compared with types");
    static_assert(std::is_convertible<Ly*, Ry*>::value || std::is_convertible<Ry*, Ly*>::value,
        "Unable to compare WeakObjPtr with raw pointer - types are incompatible");
    return static_cast<const Ry*>(l) != r.Get();
}

UTILITY_NAMESPACE_END
