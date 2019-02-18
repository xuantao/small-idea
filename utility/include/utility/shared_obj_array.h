#pragma once
#include "common.h"
#include <vector>

UTILITY_NAMESPACE_BEGIN

template <typename Ty, typename Contrutor>
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
        inline bool IsValid() const { return refCount_ < 0; }
        inline Ty* GetObj() { return reinterpret_cast<Ty*>(storage_); }
        inline const Ty* GetObj() const { return reinterpret_cast<const Ty*>(storage_); }

    public:
        union
        {
            int refCount_;  // 引用计数
            int nextIndex_; // 下一个空闲元素
        };

    private:
        union
        {
            std::max_align_t dummy_;
            int8_t storage_[sizeof(Ty)];
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
            this->refCount_ = other.refCount_;
            new (this->GetObj()) Ty(*other.GetObj());
        }

        ArrayObj& operator = (const ArrayObj&) = delete;
    };
}

/* 带引用计数的共享对象 */
template <typename Ty, typename Constructor>
class SharedObj
{
private:
    using ObjArray = SharedObjArray<Ty, Constructor>;
    friend class SharedObjArray<Ty, Constructor>;

    /* 由管理器做最初始的构造 */
    SharedObj(int index) : index_(index) { }

public:
    SharedObj() : index_(0) {}
    SharedObj(SharedObj&& obj) : index_(obj.index_) { obj.index_ = 0; }
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
        obj.index_ = 0;
        return *this;
    }

public:

    inline bool IsValid() const { return index_ > 0; }
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
        index_ = 0;
    }

private:
    void AddRef()
    {
        if (IsValid())
            ObjArray::GetObjArray()->AddRef(index_);
    }

    void UnRef()
    {
        if (IsValid())
            ObjArray::GetObjArray()->UnRef(index_);
    }

private:
    int index_;
};

/* 默认对象构造器 */
template <typename Ty>
struct DefaultSharedObjConstructor
{
    template <typename... Args>
    inline Ty* Construct(Ty* mem, Args&&... args) { return new (mem) Ty(std::forward<Args>(args)...); }

    inline void Destruct(Ty* obj) { obj->~Ty(); }
};

/* 共享对象的数组集合 */
template <typename Ty, typename Constructor = DefaultSharedObjConstructor<Ty> >
class SharedObjArray
{
    using ArrayObj = shared_obj_internal::ArrayObj<Ty, std::is_trivially_copyable<Ty>::value>;
    friend class SharedObj<Ty, Constructor>;

public:
    using value_type = Ty;
    using obj_type = SharedObj<Ty, Constructor>;

private:
    SharedObjArray(size_t incremental) : incremental_(incremental)
    {
        objs_.resize(incremental);
        for (size_t i = 0; i < incremental; ++i)
            objs_[i].nextIndex_ = (int)(i + 1);
        objs_.back().nextIndex_ = 0;
    }

    SharedObjArray(const SharedObjArray&) = delete;
    SharedObjArray& operator  = (const SharedObjArray&) = delete;

public:
    static bool Startup(size_t incremental)
    {
        assert(incremental);
        assert(s_instance_ == nullptr);
        if (s_instance_ != nullptr)
            return false;

        s_instance_ = new SharedObjArray(incremental);
        return true;
    }

    inline static SharedObjArray* GetObjArray() { return s_instance_; }

public:
    template <typename... Args>
    obj_type AllocObj(Args&&... args)
    {
        if (objs_.front().nextIndex_ == 0)
        {
            size_t old_size = objs_.size();
            size_t new_size = objs_.size() + incremental_;

            objs_.resize(new_size);
            for (size_t i = old_size; i < new_size; ++i)
                objs_[i].nextIndex_ = (int)(i + 1);
            objs_.front().nextIndex_ = (int)old_size;
            objs_.back().nextIndex_ = 0;
        }

        int index = objs_.front().nextIndex_;
        objs_.front().nextIndex_ = objs_[index].nextIndex_;

        ArrayObj* element = GetElement(index);
        element->refCount_ = -1;
        constructor_.Construct(element->GetObj(), std::forward<Args>(args)...);

        return obj_type(index);
    }

    /* traverse all valid obj
     * Func = void (value_type&)
    */
    template <typename Func>
    void Traverse(Func& func)
    {
        for (size_t i = 0; i < objs_.size(); ++i)
        {
            auto& e = objs_[i];
            if (e.IsValid())
                func(*e.GetObj());
        }
    }

private:
    inline ArrayObj* GetElement(int index)
    {
        assert(index > 0 && (size_t)index < objs_.size());
        return &objs_[index];
    }

    inline Ty* GetObj(int index) { return GetElement(index)->GetObj(); }
    inline void AddRef(int index) { GetElement(index)->refCount_ -= 1; }

    void UnRef(int index)
    {
        ArrayObj* element = GetElement(index);
        element->refCount_ += 1;

        if (element->refCount_ == 0)
        {
            constructor_.Destruct(element->GetObj());

            element->nextIndex_ = objs_.front().nextIndex_;
            objs_.front().nextIndex_ = index;
        }
    }

private:
    size_t incremental_;            // 每次扩容增量
    Constructor constructor_;       // 对象构造、销毁
    std::vector<ArrayObj> objs_;    // 对象数组

private:
    static SharedObjArray<Ty, Constructor>* s_instance_;
};

template <typename Ty, typename Constructor>
SharedObjArray<Ty, Constructor>* SharedObjArray<Ty, Constructor>::s_instance_ = nullptr;

UTILITY_NAMESPACE_END
