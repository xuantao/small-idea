#pragma once

template <typename Alloc>
struct AllocatorAdapterBase
{
    AllocatorAdapterBase(Alloc* pAlloc) : m_pAlloc(pAlloc)
    {
    }

    AllocatorAdapterBase(const AllocatorAdapterBase& o) : m_pAlloc(o.m_pAlloc)
    {
    }

    AllocatorAdapterBase& operator = (const AllocatorAdapterBase& o)
    {
        m_pAlloc = o.m_pAlloc;
        return *this;
    }

    inline Alloc* GetAlloc()
    {
        return m_pAlloc;
    }

private:
    Alloc* m_pAlloc;
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
    AllocatorAdapter() : BaseType(nullptr)
    {
    }

    AllocatorAdapter(AllocImpl* pAlloc) : BaseType(pAlloc)
    {
    }

    AllocatorAdapter(const AllocatorAdapter& o) : BaseType(o)
    {
    }

    template <typename U>
    AllocatorAdapter(const AllocatorAdapter<U, AllocImpl>& o) : BaseType(*(BaseType*)&o)
    {
    }

    AllocatorAdapter& operator = (const AllocatorAdapter& o)
    {
        *(BaseType*)this = *(BaseType*)&o;
        return *this;
    }

    template <typename U>
    AllocatorAdapter& operator = (const AllocatorAdapter<U, AllocImpl>& o)
    {
        *(BaseType*)this = *(BaseType*)&o;
        return *this;
    }

public:
    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    size_type max_size() const { return static_cast<size_type>(-1) / sizeof(value_type); }

    void construct(pointer p, const value_type& x) { new(p) value_type(x); }
    void destroy(pointer p) { p->~value_type(); }

    pointer allocate(size_type n, const_pointer = 0)
    {
        return (pointer)this->GetAlloc()->Alloc(sizeof(value_type) * n);
    }

    void deallocate(pointer p, size_type n)
    {
        this->GetAlloc()->Dealloc(p, sizeof(value_type) * n);
    }
};

template <class Ty, typename AllocImpl>
inline bool operator == (const AllocatorAdapter<Ty, AllocImpl>& l, const AllocatorAdapter<Ty, AllocImpl>& r)
{
    return l.m_pAlloc == r.m_pAlloc;
}

template <class Ty, typename AllocImpl>
inline bool operator != (const AllocatorAdapter<Ty, AllocImpl>& l, const AllocatorAdapter<Ty, AllocImpl>& r)
{
    return l.m_pAlloc != r.m_pAlloc;
}
