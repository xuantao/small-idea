/*
 * scoped buffer
 * xuantao, 2017
*/
#pragma once
#include "common.h"

UTILITY_NAMESPACE_BEGIN

/* 空白释放器 */
struct NoneDeallocator
{
    void operator() (void*, size_t) { }
};

/*
 * ScopeBuffer, 作用域范围内有效的Buffer
 * 对象用来管理和控制缓存的生命期
 * 不可以被复制/持有
*/
class ScopeBuffer
{
    template <typename _Dealloc>
    using Dealloc = CallableObject<_Dealloc, void*, size_t>;
    typedef SmallObjStorage<ICallable<void(void*, size_t)>> Storage;
public:
    ScopeBuffer() = default;

    template <typename _Dealloc>
    ScopeBuffer(void* buffer, size_t sz, _Dealloc&& dealloc)
        : buff_(buffer), size_(sz)
    {
        storage_.Construct<Dealloc<_Dealloc>>(std::forward<_Dealloc>(dealloc));
    }

    ScopeBuffer(ScopeBuffer&& other)
        : buff_(other.buff_)
        , size_(other.size_)
    {
        other.buff_ = nullptr;
        other.size_ = 0;

        if (!other.storage_.IsEmpty())
        {
            if (other.storage_.IsLocal())
            {
                other.storage_.Get()->Move(storage_.GetSpace());
            }
            else
            {
                storage_.Set(other.storage_.Get());
                other.storage_.Set(nullptr);
            }
        }
    }

    ~ScopeBuffer()
    {
        if (storage_.IsEmpty())
            return;

        storage_.Get()->Call(buff_, size_);
        storage_.Release();
    }

    ScopeBuffer(const ScopeBuffer& other) = delete;
    ScopeBuffer& operator = (const ScopeBuffer&) = delete;

public:
    inline void* get() const { return buff_; }
    inline size_t size() const { return size_; }
    inline bool empty() const { return get() == nullptr; }
    inline explicit operator bool() const { return buff_ != nullptr; }

    template <typename Ty>
    inline Ty& as() { *((Ty*)buff_); }

protected:
    void* buff_ = nullptr;
    size_t size_ = 0;
    Storage storage_;
};

UTILITY_NAMESPACE_END
