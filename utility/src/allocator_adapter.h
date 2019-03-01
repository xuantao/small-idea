/* adpate std allocator */
#pragma once

UTILITY_NAMESPACE_BEGIN

template <typename Alloc>
struct AllocatorAdapterBase
{
    AllocatorAdapterBase(Alloc* alloc) : alloc_(alloc) { }
    AllocatorAdapterBase(const AllocatorAdapterBase& other) : alloc_(other.alloc_) { }

    inline AllocatorAdapterBase& operator = (const AllocatorAdapterBase& other)
    {
        alloc_ = other.alloc_;
        return *this;
    }

    inline Alloc* GetAlloc() { return alloc_; }

private:
    Alloc* alloc_;
};

/* 适配标准库的分配器 */
template <typename Ty, class AllocImpl>
class AllocatorAdapter : protected AllocatorAdapterBase<AllocImpl>
{
    typedef AllocatorAdapterBase<AllocImpl> BaseType;
public:
    typedef Ty                value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;

    template <class U>
    struct rebind { typedef AllocatorAdapter<U, AllocImpl> other; };

public:
    AllocatorAdapter() : BaseType(nullptr) { }
    AllocatorAdapter(AllocImpl* alloc) : BaseType(alloc) { }
    AllocatorAdapter(const AllocatorAdapter& other) : BaseType(other) { }

    template <typename U>
    AllocatorAdapter(const AllocatorAdapter<U, AllocImpl>& other) : BaseType(*(BaseType*)&other)
    { }

    inline AllocatorAdapter& operator = (const AllocatorAdapter& other)
    {
        *(BaseType*)this = *(BaseType*)&other;
        return *this;
    }

    template <typename U>
    inline AllocatorAdapter& operator = (const AllocatorAdapter<U, AllocImpl>& other)
    {
        *(BaseType*)this = *(BaseType*)&other;
        return *this;
    }

public:
    inline pointer address(reference x) const { return &x; }
    inline const_pointer address(const_reference x) const { return &x; }

    inline size_type max_size() const { return static_cast<size_type>(-1) / sizeof(value_type); }

    inline void construct(pointer p, const value_type& x) { new(p) value_type(x); }
    inline void destroy(pointer p) { p->~value_type(); }

    inline pointer allocate(size_type n, const_pointer = 0)
    {
        return (pointer)this->GetAlloc()->Alloc(sizeof(value_type) * n);
    }

    inline void deallocate(pointer p, size_type n)
    {
        this->GetAlloc()->Dealloc(p, sizeof(value_type) * n);
    }
};

template <class Ty, typename AllocImpl>
inline bool operator == (const AllocatorAdapter<Ty, AllocImpl>& l, const AllocatorAdapter<Ty, AllocImpl>& r)
{
    return l.alloc_ == r.alloc_;
}

template <class Ty, typename AllocImpl>
inline bool operator != (const AllocatorAdapter<Ty, AllocImpl>& l, const AllocatorAdapter<Ty, AllocImpl>& r)
{
    return l.alloc_ != r.alloc_;
}

UTILITY_NAMESPACE_END
