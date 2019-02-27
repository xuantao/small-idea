#pragma once
#include "common.h"
#include <vector>

UTILITY_NAMESPACE_BEGIN

template <typename Ty>
class SharedObjArray;

namespace shared_obj_internal
{
    /* 实际数据元素
     * refCount < 0: 数组元素持有对象
    */
    template <typename Ty>
    struct ArrayObjBase
    {
    public:
        inline bool IsValid() const { return ref_count < 0; }
        inline Ty* GetObj() { return reinterpret_cast<Ty*>(storage); }
        inline const Ty* GetObj() const { return reinterpret_cast<const Ty*>(storage); }
        inline void* GetMem() { return static_cast<void*>(storage); }

    public:
        /* 记录引用计数或下一个空闲位置索引
         * 引用计数使用复数，空闲缩影使用正数
        */
        union
        {
            int ref_count;  // 引用计数
            int next_index; // 下一个空闲元素
        };

    private:
        union
        {
            std::max_align_t dummy_;
            int8_t storage[sizeof(Ty)];
        };
    };

    template <typename Ty, bool>
    struct ArrayObj : public ArrayObjBase<Ty> { };

    /* 非平凡复制构造 */
    template <typename Ty>
    struct ArrayObj<Ty, false> : public ArrayObjBase<Ty>
    {
        ArrayObj() = default;

        ArrayObj(const ArrayObj& other)
        {
            this->ref_count = other.ref_count;
            new (this->GetMem()) Ty(*other.GetObj());
        }

        ArrayObj& operator = (const ArrayObj&) = delete;
    };
} // namespace shared_obj_internal

/* 带引用计数的共享对象 */
template <typename Ty>
class SharedObj
{
private:
    using ObjArray = SharedObjArray<Ty>;
    friend class SharedObjArray<Ty>;

    /* 由管理器做最初始的构造 */
    SharedObj(int index) : index_(index) { }

public:
    SharedObj() : index_(const_values::kInvalidIndex) {}
    SharedObj(SharedObj&& obj) : index_(obj.index_) { obj.index_ = const_values::kInvalidIndex; }
    SharedObj(const SharedObj& obj) : index_(obj.index_) { AddRef(); }
    ~SharedObj() { UnRef(); }

public:
    inline SharedObj& operator = (const SharedObj& obj)
    {
        UnRef();
        index_ = obj.index_;
        AddRef();
        return *this;
    }

    inline SharedObj& operator = (SharedObj&& obj)
    {
        UnRef();
        index_ = obj.index_;
        obj.index_ = const_values::kInvalidIndex;
        return *this;
    }

    inline bool operator == (const SharedObj& obj) const { return index_ == obj.index_; }
    inline bool operator != (const SharedObj& obj) const { return !(*this == obj); }

public:
    inline bool IsValid() const { return index_ >= 0; }
    inline explicit operator bool() { return IsValid(); }

    inline Ty* GetObj() const
    {
        if (IsValid())
            return ObjArray::GetObjArray()->GetObj(index_);
        return nullptr;
    }

    inline void Release()
    {
        UnRef();
        index_ = const_values::kInvalidIndex;
    }

private:
    inline void AddRef()
    {
        if (IsValid())
            ObjArray::GetObjArray()->AddRef(index_);
    }

    inline void UnRef()
    {
        if (IsValid())
            ObjArray::GetObjArray()->UnRef(index_);
    }

private:
    int index_;
}; // class SharedObj

/* 共享对象的数组集合 */
template <typename Ty>
class SharedObjArray
{
    using ArrayObj = shared_obj_internal::ArrayObj<Ty, std::is_trivially_copyable<Ty>::value>;
    friend class SharedObj<Ty>;

public:
    using value_type = Ty;
    using obj_type = SharedObj<Ty>;

private:
    SharedObjArray(size_t incremental)
        : free_index_(const_values::kInvalidIndex)
        , incremental_(incremental)
    {
    }

    ~SharedObjArray()
    {
        for (auto& e : objs_)
        {
            if (e.IsValid())
                e.GetObj()->~Ty();
        }

        objs_.clear();
    }

    SharedObjArray(const SharedObjArray&) = delete;
    SharedObjArray& operator = (const SharedObjArray&) = delete;

public:
    static bool Startup(size_t incremental = 1024)
    {
        assert(incremental);
        assert(s_instance_ == nullptr);
        if (s_instance_ != nullptr)
            return false;

        s_instance_ = new SharedObjArray(incremental);
        return true;
    }

    static void Shutdown()
    {
        delete s_instance_;
        s_instance_ = nullptr;
    }

    inline static SharedObjArray* GetObjArray() { return s_instance_; }

public:
    /* alloc object with constructor paramenters */
    template <typename... Args>
    obj_type AllocObj(Args&&... args)
    {
        if (free_index_ == const_values::kInvalidIndex)
        {
            size_t old_size = objs_.size();
            size_t new_size = objs_.size() + incremental_;

            objs_.resize(new_size);
            for (size_t i = old_size; i < new_size; ++i)
                objs_[i].next_index = (int)(i + 1);

            objs_.back().next_index = const_values::kInvalidIndex;
            free_index_ = (int)old_size;
        }

        int index = free_index_;
        ArrayObj& aryObj = objs_[index];
        free_index_ = aryObj.next_index;

        aryObj.ref_count = -1;
        new (aryObj.GetMem()) Ty(std::forward<Args>(args)...);
        return obj_type(index);
    }

    /* traverse all valid obj
     * Func = void (value_type&)
    */
    template <typename Func>
    void Traverse(Func& func)
    {
        for (auto& e : objs_)
        {
            if (e.IsValid())
                func(*e.GetObj());
        }
    }

private:
    inline ArrayObj& GetAryObj(int index)
    {
        assert(index >= 0 && (size_t)index < objs_.size());
        return objs_[index];
    }

    inline Ty* GetObj(int index) { return GetAryObj(index).GetObj(); }
    inline void AddRef(int index) { GetAryObj(index).ref_count -= 1; }

    inline void UnRef(int index)
    {
        ArrayObj& aryObj = GetAryObj(index);
        aryObj.ref_count += 1;

        if (aryObj.ref_count == 0)
        {
            aryObj.GetObj()->~Ty();
            aryObj.next_index = free_index_;
            free_index_ = index;
        }
    }

private:
    int free_index_;
    size_t incremental_;            // 每次扩容增量
    std::vector<ArrayObj> objs_;    // 对象数组

private:
    static SharedObjArray<Ty>* s_instance_;
};

template <typename Ty>
SharedObjArray<Ty>* SharedObjArray<Ty>::s_instance_ = nullptr;

UTILITY_NAMESPACE_END
