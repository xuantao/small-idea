#include <cassert>
#include <vector>
#include "small_obj_storage.h"

UTILITY_NAMESPACE_BEGIN

template <typename Ty>
struct CompactListData
{
    struct Node {
        unsigned prev_;
        unsigned next_;
        ObjStorage<Ty> storage_;
    };

    unsigned size_ = 0;
    unsigned free_ = 0;
    std::vector<Node> data_{{0, 0}};
};

template <typename Ty>
struct CompactListIterator : public std::iterator<std::bidirectional_iterator_tag, Ty> {
    using _DataType = CompactListData<Ty>;
    using _MyBase = std::iterator<std::bidirectional_iterator_tag, Ty>;
    using pointer = typename _MyBase::pointer;
    using reference = typename _MyBase::reference;

    CompactListIterator() = default;
    CompactListIterator(_DataType* lst, unsigned pos) : list_(lst), pos_(pos) {}

    inline reference operator*() const {
        return *list_->data_[pos_].storage_.Get();
    }

    inline pointer operator->() const {
        return (std::pointer_traits<pointer>::pointer_to(**this));
    }

    inline CompactListIterator& operator++() {
        pos_ = list_->data_[pos_].next_;
        return (*this);
    }

    inline CompactListIterator operator++(int) {
        CompactListIterator tmp = *this;
        ++*this;
        return (tmp);
    }

    inline CompactListIterator& operator--() {
        pos_ = list_->data_[pos_].prev_;
        return (*this);
    }

    inline CompactListIterator operator--(int) {
        CompactListIterator tmp = *this;
        --*this;
        return (tmp);
    }

    inline bool operator==(const CompactListIterator& other) const {
        return pos_ == other.pos_;
    }

    inline bool operator!=(const CompactListIterator& other) const {
        return !(*this == other);
    }

    _DataType* list_ = nullptr;
    unsigned pos_ = 0;
};

template <typename Ty>
struct CompactListConstIterator : public std::iterator<std::bidirectional_iterator_tag, const Ty> {
    using _DataType = CompactListData<Ty>;
    using _MyBase = std::iterator<std::bidirectional_iterator_tag, const Ty>;
    using pointer = typename _MyBase::pointer;
    using reference = typename _MyBase::reference;

    CompactListConstIterator() = default;
    CompactListConstIterator(const _DataType* lst, unsigned pos) : list_(lst), pos_(pos) {}

    inline reference operator*() const {
        return *list_->data_[pos_].storage_.Get();
    }

    inline pointer operator->() const {
        return (std::pointer_traits<pointer>::pointer_to(**this));
    }

    inline CompactListConstIterator& operator++() {
        pos_ = list_->data_[pos_].next_;
        return (*this);
    }

    inline CompactListConstIterator operator++(int) {
        CompactListConstIterator tmp = *this;
        ++*this;
        return (tmp);
    }

    inline CompactListConstIterator& operator--() {
        pos_ = list_->data_[pos_].prev_;
        return (*this);
    }

    inline CompactListConstIterator operator--(int) {
        CompactListConstIterator tmp = *this;
        --*this;
        return (tmp);
    }

    inline bool operator==(const CompactListConstIterator& other) const {
        return pos_ == other.pos_;
    }

    inline bool operator!=(const CompactListConstIterator& other) const {
        return !(*this == other);
    }

    const _DataType* list_ = nullptr;
    unsigned pos_ = 0;
};

template <typename Ty, typename Alloc = std::allocator<Ty>>
class CompactList : private CompactListData<Ty> {
public:
    using allocator_type = Alloc;
    using value_type = Ty;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = CompactListIterator<Ty>;
    using const_iterator = CompactListConstIterator<Ty>;

public:
    CompactList() = default;
    CompactList(const CompactList& other) = default;
    CompactList(CompactList&& other) { swap(other); }

    inline CompactList& operator = (const CompactList& other) = default;
    inline CompactList& operator = (CompactList&& other) { swap(other); }

public:
    inline size_t size() const { return this->size_; }
    inline bool empty() const { return this->data_[0].next_ == 0; }
    inline iterator begin() { return iterator(this, this->data_[0].next_); }
    inline iterator end() { return iterator(this, 0); }
    inline const_iterator cbegin() const { return const_iterator(this, this->data_[0].next_); }
    inline const_iterator cend() const { return const_iterator(this, 0); }

    inline reference front() { return *begin(); }
    inline const_reference front() const { return *cbegin(); }
    inline reference back() { return *(--end()); }
    inline const_reference back() const { return *(--cend()); }

public:
    inline void push_front(const Ty& val) {
        _Insert(this->data_[0].next_, val);
    }

    inline void push_front(Ty&& val) {
        _Insert(this->data_[0].next_, std::move(val));
    }

    inline void push_back(const Ty& val) {
        _Insert(0, val);
    }

    inline void push_back(Ty&& val) {
        _Insert(0, std::move(val));
    }

    inline void insert(iterator it, const Ty& val) {
        _Insert(it.pos_, val);
    }

    inline void insert(iterator it, Ty&& val) {
        _Insert(it.pos_, std::move(val));
    }

    inline void insert(const_iterator it, const Ty& val) {
        _Insert(it.pos_, val);
    }

    inline void insert(const_iterator it, Ty&& val) {
        _Insert(it.pos_, std::move(val));
    }

    inline void erase(iterator it) {
        if (it.pos_) _Free_node(it.pos_);
    }

    inline void erase(const_iterator it) {
        if (it.pos_) _Free_node(it.pos_);
    }

    inline void swap(CompactList& other) {
        unsigned s = this->size_;
        unsigned f = this->free_;
        this->size_ = other.size_;
        this->free_ = other.free_;
        this->data_.swap(other.data_);
        other.size_ = s;
        other.free_ = f;
    }

private:
    unsigned _Buy_node() {
        unsigned idx = this->free_;
        if (idx) {
            this->free_ = this->data_[this->free_].next_;
        } else {
            idx = (unsigned)this->data_.size();
            this->data_.emplace_back();
        }
        ++this->size_;
        return idx;
    }

    inline void _Free_node(unsigned pos) {
        auto& node = this->data_[pos];
        this->data_[node.prev_].next_ = node.next_;
        this->data_[node.next_].prev_ = node.prev_;
        node.next_ = this->free_;
        node.storage_.Release();
        this->free_ = pos;
        --this->size_;
    }

    template <typename _Ty>
    inline void _Insert(unsigned pos, _Ty&& val) {
        unsigned at = _Buy_node();
        auto& pos_node = this->data_[pos];
        auto& node = this->data_[at];
        node.storage_.Construct(std::forward<_Ty>(val));
        node.next_ = pos;
        node.prev_ = pos_node.prev_;
        pos_node.prev_ = at;
        this->data_[node.prev_].next_ = at;
    }
};

UTILITY_NAMESPACE_END
