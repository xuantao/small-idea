/*
 * ObjectIndex & ObjectArray
 * ObjectIndex: record the index in object array
 * ObjectArray: the array with object pointer and serial-number(use for check the pointer is valid)
*/
#pragma once
#include "common.h"
#include "singleton.h"
#include <vector>

/* weak obj index member name */
#ifndef OBJECT_INDEX_MEMBER_NAME
    #define OBJECT_INDEX_MEMBER_NAME obj_index
#endif // !OBJECT_INDEX_MEMBER_NAME

/* 声明ObjectIndex成员变量用于支持弱引用指针 */
#define DECLARE_OBJECT_INDEX_MEMBER UTILITY_NAMESPACE ObjectIndex OBJECT_INDEX_MEMBER_NAME

/* 编译期检测错误输出 */
#define _SUPPORT_WEAK_OBJ_PTR_ERROR "do not support WeakObjPtr, the type must be public inherit from ObjectIndex or has public ObjectIndex Member"

UTILITY_NAMESPACE_BEGIN

/* 对象编号
 * 记录对象存储在对象数组中的编号
 * 构建对象的弱引用关系时将向ObjectArray中分配一个slot编号
*/
class ObjectIndex
{
    friend class WeakObjArray;
public:
    ObjectIndex() : index_(vals::kInvalidIndex) { }
    ~ObjectIndex();

    ObjectIndex(const ObjectIndex&) = delete;
    ObjectIndex& operator = (const ObjectIndex&) = delete;

private:
    int index_;
};

/* 检测是否含有ObjectIndex的成员变量 */
template <typename Ty>
struct HasObjectIndexMember
{
    template<typename U> static auto Check(int) -> decltype(std::declval<U>().OBJECT_INDEX_MEMBER_NAME);
    template<typename U> static std::false_type Check(...);

    static constexpr bool value = std::is_same<decltype(Check<Ty>(0)), ObjectIndex>::value;
};

/* 检测指定类型是否还有ObjectIndex, 判断是否支持ObjectArray
 * 1. 类型从ObjectIndex公共继承
 * 2. 拥有共有的[ObjectIndex obj_index;]成员变量
*/
template <typename Ty>
struct HasObjectIndex : std::bool_constant<std::is_base_of<ObjectIndex, Ty>::value || HasObjectIndexMember<Ty>::value>
{
};

class WeakObjArray : public Singleton<WeakObjArray>
{
private:
    /* 数组元素对象 */
    struct ArrayObj
    {
        int serial_num; // 序列号
        int next_index; // 下一个空闲元素
        void* obj;      // 对象指针
    };

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
        static_assert(HasObjectIndex<Ty>::value, _SUPPORT_WEAK_OBJ_PTR_ERROR);
        assert(obj);
        if (obj == nullptr)
            return vals::kInvalidIndex;

        auto& weak_index = GetIndex(obj);
        if (weak_index.index_ != vals::kInvalidIndex)
            return weak_index.index_;

        // 无可用元素，增加数组容量
        if (free_index == vals::kInvalidIndex)
        {
            int old_size = (int)obj_.size();
            int new_size = old_size + inc_;

            free_index = old_size;
            obj_.resize(new_size);
            for (int i = old_size; i < new_size; ++i)
            {
                auto& o = obj_[i];
                o.next_index = i + 1;
                o.serial_num = 0;
                o.obj = nullptr;
            }
            obj_.back().next_index = vals::kInvalidIndex;
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
        static_assert(HasObjectIndex<Ty>::value, _SUPPORT_WEAK_OBJ_PTR_ERROR);
        FreeIndex(GetIndex(obj));
    }

    inline void FreeIndex(ObjectIndex& weak_index)
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

    inline void* GetObject(int index) const
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
    inline ObjectIndex& GetIndex(Ty* obj)
    {
        return GetIndex(std::is_base_of<ObjectIndex, Ty>(), obj);
    }

    template <typename Ty>
    inline ObjectIndex& GetIndex(std::true_type, Ty* obj)
    {
        return static_cast<ObjectIndex&>(*obj);
    }

    template <typename Ty>
    inline ObjectIndex& GetIndex(std::false_type, Ty* obj)
    {
        return obj->OBJECT_INDEX_MEMBER_NAME;
    }

private:
    int inc_;                   // 数组容量每次增量
    int serial_num_gener_;      // 数组元素序列号
    int free_index;             // 下一个空闲元素位置
    std::vector<ArrayObj> obj_; // 实际数组
};

ObjectIndex::~ObjectIndex()
{
    WeakObjArray::GetInstance()->FreeIndex(*this);
}

UTILITY_NAMESPACE_END
