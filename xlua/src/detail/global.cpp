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

ITypeDesc* AllocTypeInfo(const char* name, const TypeInfo* super)
{
    if (detail::s_global == nullptr)
        return nullptr;
    return detail::s_global->AllocType(name, super);
}

const TypeInfo* GetTypeInfo(const TypeKey& key)
{
    if (detail::s_global == nullptr)
        return nullptr;
    return detail::s_global->GetTypeInfo(key);
}

ObjIndex::~ObjIndex()
{
    if (index_ != -1 && detail::s_global)
    {
        ;   //TODO: release
    }
}

bool TypeKey::IsValid() const
{
    return detail::s_global && detail::s_global->IsValid(*this);
}

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

    bool GlobalVar::IsValid(const TypeKey& key) const
    {
        return key.serial_ == version_
            && key.index_ >= 0
            && key.index_ < (int)types_.size();
    }

    ITypeDesc* GlobalVar::AllocType(const char* name, const TypeInfo* super)
    {
        return new TypeDesc(s_global, name, super);
    }

    const TypeInfo* GlobalVar::GetTypeInfo(const TypeKey& key) const
    {
        if (!IsValid(key))
            return nullptr;

        return types_[key.index_];
    }

    TypeKey GlobalVar::AddTypeInfo(TypeInfo* info)
    {
        TypeKey key;
        key.serial_ = version_;
        key.index_ = (int)types_.size();
        types_.push_back(info);
        return key;
    }

} // namespace detail
//
//struct LuaExpType
//{
//public:
//    typedef Declare LuaDeclare;
//    static const TypeInfo* LuaGetTypeInfo();
//};
//
//namespace
//{
//    static int LuaExpType_Index = -1;
//    struct LuaExpType_Node : private xlua_internal::TypeNode
//    {
//        LuaExpType_Node() : xlua_internal::TypeNode(&LuaExpType::LuaGetTypeInfo) { }
//        ~LuaExpType_Node() { LuaExpType_Index = -1; }
//    } LuaExpType_Node_;
//}
//
//const TypeInfo* LuaExpType::LuaGetTypeInfo()
//{
//    //TODO:
//    if (LuaExpType_Index != -1)
//        return xlua_internal::GetTypeInfo(LuaExpType_Index);
//
//    ITypeDesc* desc = xlua_internal::AllocTypeInfo();
//    if (desc == nullptr) return nullptr;
//
//    //TODO:
//    desc->SetName("");
//    desc->SetSupper(nullptr);
//    desc->AddFunc("", nullptr, false);
//
//    LuaExpType_Index = desc->Finalize();
//    return xlua_internal::GetTypeInfo(LuaExpType_Index);
//}

XLUA_NAMESPACE_END

enum TestEnum
{
    kValue1,
    kValue2,
};

enum class TestEnum2
{
    kValue11,
    kValue12,
};

struct TestLuaExport
{
    int ia;
    void test_call() {}
};


XLUA_EXPORT_ENUM_BEGIN(TestEnum2)
XLUA_EXPORT_ENUM_VAR(kValue11)
XLUA_EXPORT_ENUM_VAR_AS(ttt, kValue12)
XLUA_EXPORT_ENUM_END()

XLUA_DECLARE_EXPORT_EXTERNAL_CLASS(TestLuaExport);

XLUA_EXPORT_EXTERNAL_CLASS_BEGIN(TestLuaExport)
XLUA_EXPORT_MEMBER_FUNC(test_call)
XLUA_EXPORT_MEMBER_VAR(ia)
XLUA_EXPORT_EXTERNAL_CLASS_END()
