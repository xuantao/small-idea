#pragma once
#include "common.h"
#include <vector>

UTILITY_NAMESPACE_BEGIN

class WeakObjIndex
{
    friend class WeakObjArray;
public:
    WeakObjIndex() : index_(const_values::kInvalidIndex) { }
    ~WeakObjIndex();

    WeakObjIndex(const WeakObjIndex&) = delete;
    WeakObjIndex& operator = (const WeakObjIndex&) = delete;

private:
    int index_;
};

namespace weak_obj_internal
{
    struct ArrayObj
    {
        /* 记录引用计数或下一个空闲位置索引
         * 引用计数使用复数，空闲缩影使用正数
        */
        union
        {
            int serial_num;     // 序列号
            int next_index;     // 下一个空闲元素
        };

        void* obj;
    };
}

class WeakObjArray
{
    using ArrayObj = weak_obj_internal::ArrayObj;
public:
    WeakObjArray() : serial_num_gener_(0), free_index(const_values::kInvalidIndex) { }

public:
    template <typename Ty>
    int AllocIndex(Ty* obj);

    template <typename Ty>
    void FreeIndex(Ty* obj);

    int AllocSerialNum(int index);
    int GetSerialNum(int index) const;

private:
    int serial_num_gener_;
    int free_index;
    std::vector<ArrayObj> obj_;
};

namespace weak_obj_internal
{
    class WeakObjBase
    {
    public:
        WeakObjBase() : index_(const_values::kInvalidIndex), serial_num_(0) { }
        WeakObjBase(const WeakObjBase& other) = default;
        WeakObjBase& operator = (const WeakObjBase& other) = default;

    public:
        bool IsValid() const;
        void* Get() const;
        void Set(WeakObjIndex& obj_index, void* obj);

        inline void Reset()
        {
            index_ = const_values::kInvalidIndex;
            serial_num_ = 0;
        }

    private:
        int index_;
        int serial_num_;
    };

    template <typename Ty>
    struct HasWeakIndex
    {
        template<typename U> static auto Check(int) -> decltype(std::declval<U>().weak_obj_index);
        template<typename U> static std::false_type Check(...);

        static constexpr bool value = std::is_same<decltype(Check<Ty>(0)), WeakObjIndex>::value;
    };

    template <typename Ty>
    inline void DoSet(std::true_type, WeakObjBase* ptr, Ty* obj) { ptr->Set(*obj, obj); }

    template <typename Ty>
    inline void DoSet(std::false_type, WeakObjBase* ptr, Ty* obj) { ptr->Set(obj->weak_obj_index, obj); }

} // namespace weak_obj_internal

template <typename Ty>
class WeakObjPtr : private weak_obj_internal::WeakObjBase
{
    static_assert(std::is_base_of<WeakObjIndex, Ty>::value || weak_obj_internal::HasWeakIndex<Ty>::value, "not support weak obj");
    using Super = weak_obj_internal::WeakObjBase;
public:
    WeakObjPtr() = default;
    WeakObjPtr(const WeakObjPtr&) = default;
    WeakObjPtr(std::nullptr_t) { }

    template <typename U>
    WeakObjPtr(U* obj/*, int = 0*/) { this->operator = (obj); }

    template <typename U>
    WeakObjPtr(const WeakObjPtr<U>& other) { this->operator = (other); }

public:
    WeakObjPtr& operator = (const WeakObjPtr&) = default;

    template <typename U>
    inline void operator = (U* obj)
    {
        Ty* tmp = obj;
        weak_obj_internal::DoSet(std::is_base_of<WeakObjIndex, Ty>(), this, obj);
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

UTILITY_NAMESPACE_END
