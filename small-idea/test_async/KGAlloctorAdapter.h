#pragma once

/* 适配标准库的分配器 */
template <typename Ty, typename AllocImpl>
class KGAlloctorAdapter
{
public:
    typedef Ty                value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef std::size_t       size_type;
    typedef std::ptrdiff_t    difference_type;

    template <class U>
    struct rebind { typedef KGAlloctorAdapter<U, AllocImpl> other; };

protected:
    KGAlloctorAdapter(AllocImpl* pAlloc) : m_pAlloc(pAlloc)
    {
    }

public:
    KGAlloctorAdapter(const KGAlloctorAdapter& o) : m_pAlloc(o.pAlloc)
    {
    }

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    size_type max_size() const { return static_cast<size_type>(-1) / sizeof(value_type); }

    void construct(pointer p, const value_type& x) { new(p) value_type(x); }
    void destroy(pointer p) { p->~value_type(); }

    pointer allocate(size_type n, const_pointer = 0)
    {
        return m_pAlloc->Alloc(sizeof(value_type) * n);
    }

    void deallocate(pointer p, size_type n)
    {
        m_pAlloc->Dealloc(p, sizeof(value_type) * n);
    }

private:
    AllocImpl* m_pAlloc;
};

template <class Ty, typename AllocImpl>
inline bool operator == (const KGAlloctorAdapter<Ty, AllocImpl>&, const KGAlloctorAdapter<Ty, AllocImpl>&)
{
    return true;
}

template <class Ty, typename AllocImpl>
inline bool operator != (const KGAlloctorAdapter<Ty, AllocImpl>&, const KGAlloctorAdapter<Ty, AllocImpl>&)
{
    return false;
}
