#include "global.h"
#include "type_desc.h"
#include "../xlua_export.h"
#include <memory>

XLUA_NAMESPACE_BEGIN

namespace detail
{
    static int s_version = 0;
    static NodeBase* s_node_head = nullptr;
    static GlobalVar* s_global = nullptr;
}

ITypeDesc* AllocTypeInfo(TypeCategory category, bool is_weak_obj, const char* name, const TypeInfo* super)
{
    if (detail::s_global == nullptr)
        return nullptr;
    return detail::s_global->AllocType(category, is_weak_obj, name, super);
}

//const TypeInfo* GetTypeInfo(const TypeKey& key)
//{
//    if (detail::s_global == nullptr)
//        return nullptr;
//    return detail::s_global->GetTypeInfo(key);
//}

ObjIndex::~ObjIndex()
{
    if (index_ != -1 && detail::s_global)
        detail::s_global->FreeObjIndex(index_);
}

//bool TypeKey::IsValid() const
//{
//    return detail::s_global && detail::s_global->IsValid(*this);
//}

namespace detail
{
    static void* DummyCast(void*, const TypeInfo*, const TypeInfo*) { return nullptr; }

    const char* PerifyTypeName(const char* name)
    {
        if (name == nullptr || *name == 0)
            return name;

        size_t length = ::strlen(name);
        char* buff = new char[length + 1];
        strcpy_s(buff, length + 1, name);

        char* sub = nullptr;
        char* str = const_cast<char*>(buff);
        while ((sub = ::strstr(str, "::")) != nullptr)
        {
            ::memmove(sub, sub + 2, length - (sub - buff) - 2);
            str = sub;
        }
        return name;
    }

    NodeBase::NodeBase(NodeType type) : type_(type)
    {
        next_ = s_node_head;
        s_node_head = this;

        if (GlobalVar::GetInstance())
        {
            //TODO: 注册
        }
    }

    NodeBase::~NodeBase()
    {
        NodeBase* node = s_node_head;
        while (node != this && node->next_ != this)
            node = node->next_;

        if (node == this)
            s_node_head = next_;
        else
            node->next_ = next_;
        next_ = nullptr;

        if (GlobalVar::GetInstance())
        {
            //TODO: 反注册
        }
    }

    bool GlobalVar::Startup()
    {
        if (s_global)
            return false;

        s_global = new GlobalVar(++s_version);

        NodeBase* node = s_node_head;
        while (node) {
            switch (node->type_) {
            case NodeType::kType:
                static_cast<TypeNode*>(node)->func_();
                break;
            case NodeType::kConst:
                static_cast<ConstNode*>(node)->func_();
                break;
            case NodeType::kScript:
                break;
            default:
                break;
            }

            node = node->next_;
        }
        return true;
    }

    GlobalVar* GlobalVar::GetInstance()
    {
        return s_global;
    }

    void GlobalVar::Purge()
    {
        delete s_global;
        s_global = nullptr;
    }

    GlobalVar::GlobalVar(int version) : version_(version)
    {
    }

    GlobalVar::~GlobalVar()
    {
    }

    xLuaState* GlobalVar::Create() {
        return nullptr;
    }

    xLuaState* GlobalVar::Attach(lua_State* l) {
        return nullptr;
    }

    void GlobalVar::Destory(xLuaState* l) {

    }

    bool GlobalVar::IsValid(const TypeKey& key) const
    {
        return key.serial_ == version_
            && key.index_ >= 0
            && key.index_ < (int)types_.size();
    }

    ITypeDesc* GlobalVar::AllocType(TypeCategory category, bool is_weak_obj, const char* name, const TypeInfo* super)
    {
        return new TypeDesc(s_global,category, is_weak_obj , name, super);
    }

    const TypeInfo* GlobalVar::GetTypeInfo(const TypeKey& key) const
    {
        if (!IsValid(key))
            return nullptr;

        return types_[key.index_];
    }

    int GlobalVar::AllocObjIndex(ObjIndex& obj_index, void* obj, const TypeInfo* info) {
        if (obj_index.index_ != -1) {
            // 总是存储子类指针
            auto& ary_obj = obj_array_[obj_index.index_];
            if (!IsBaseOf(info, ary_obj.info_)) {
                ary_obj.obj_ = obj;
                ary_obj.info_ = info;
            }
            return obj_index.index_;
        } else {
            // 无可用元素，增加数组容量
            if (free_index_.empty()) {
                size_t old_size = obj_array_.size();
                size_t new_size = obj_array_.size() + 1024;
                obj_array_.resize(new_size, ArrayObj{0, nullptr, nullptr});

                if (free_index_.capacity() < 1024)
                    free_index_.reserve(1024);

                for (size_t i = new_size; i > old_size; --i)
                    free_index_.push_back((int)i - 1);
            }

            auto index = free_index_.back();
            auto& ary_obj = obj_array_[index];
            free_index_.pop_back();

            ary_obj.info_ = info;
            ary_obj.obj_ = obj;
            ary_obj.serial_num_ = ++serial_num_gener_;

            obj_index.index_ = index;
            return index;
        }
    }

    ArrayObj* GlobalVar::GetArrayObj(int index) {
        if (index < 0 || index >= (int)obj_array_.size())
            return nullptr;
        return &obj_array_[index];
    }

    //int GlobalVar::GetObjSerialNum(int index) {
    //    return 0;
    //}

    TypeKey GlobalVar::AddTypeInfo(TypeInfo* info)
    {
        TypeKey key;
        key.serial_ = version_;
        key.index_ = (int)types_.size();
        types_.push_back(info);
        return key;
    }

    void GlobalVar::FreeObjIndex(int index)
    {
        auto& ary_obj = obj_array_[index];
        ary_obj.info_ = nullptr;
        ary_obj.obj_ = nullptr;
        ary_obj.serial_num_ = 0;

        if ((free_index_.capacity() - free_index_.size()) == 0)
            free_index_.reserve(free_index_.size() + 1024);
        free_index_.push_back(index);
    }

} // namespace detail

XLUA_NAMESPACE_END
