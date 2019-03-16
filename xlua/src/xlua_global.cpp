#include "xlua_global.h"
#include "xlua_export.h"
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
        ;   //TODO: release
}

bool TypeKey::IsValid() const
{
    return detail::s_global && detail::s_global->IsValid(*this);
}

namespace detail
{
    static bool DummyCast(void*, const TypeInfo*, const TypeInfo*) { return false; }

    const char* PerifyTypeName(const char* name)
    {
        if (name == nullptr || *name == 0)
            return name;

        size_t length = ::strlen(name);
        char* buff = new char[length + 1];
        strcpy_s(buff, length, name);

        char* sub = nullptr;
        char* str = const_cast<char*>(buff);
        while ((sub = ::strstr(str, "::")) != nullptr)
        {
            ::memmove(sub, sub + 2, length - (sub - buff) - 2);
            str = sub;
        }
        return name;
    }

    struct TypeDesc : public ITypeDesc
    {
    public:
        TypeDesc(const char* name, const TypeInfo* super) : name_(name), super_(super) { }
        ~TypeDesc() { }

        void SetCastCheck(LuaCastCheck cast_checker) override
        {
            cast_checker_ = cast_checker;
        }

        void AddFunc(const char* name, LuaFunction func, bool global) override
        {
            TypeFunc mf{ name, func };
            if (global)
                static_member_func_.push_back(mf);
            else
                member_func_.push_back(mf);
        }

        void AddVar(const char* name, LuaIndexer getter, LuaIndexer setter, bool global) override
        {
            TypeVar mv{ name, getter, setter };
            if (global)
                static_var_func_.push_back(mv);
            else
                member_var_.push_back(mv);
        }

        TypeKey Finalize() override
        {
            std::auto_ptr<TypeDesc> ptr(this);  // auto free
            if (s_global == nullptr)
                return TypeKey();

            void* buff = new char[sizeof(TypeInfo)
                + sizeof(TypeFunc) * (member_func_.size() + static_member_func_.size() + 2)
                + sizeof(TypeVar) * (member_var_.size() + static_var_func_.size() + 2)
            ];
            TypeInfo* info = static_cast<TypeInfo*>(buff);

            info->name = PerifyTypeName(name_);
            info->super = super_;
            info->fnCast = cast_checker_ ? cast_checker_ : &DummyCast;

            TypeFunc* func = reinterpret_cast<TypeFunc*>(&info[1]);

            info->funcs = func;
            for (const auto& f : member_func_)
            {
                *func = f;
                ++func;
            }
            func->func = nullptr;
            func->name = nullptr;
            ++func;

            info->static_funcs = func;
            for (const auto& f : static_member_func_)
            {
                *func = f;
                ++func;
            }
            func->func = nullptr;
            func->name = nullptr;
            ++func;

            TypeVar* var = reinterpret_cast<TypeVar*>(func);
            info->vars = var;
            for (const auto& v : member_var_)
            {
                *var = v;
                ++var;
            }
            var->name = nullptr;
            var->getter = nullptr;
            var->setter = nullptr;
            ++var;

            info->static_vars = var;
            for (const auto& v : static_var_func_)
            {
                *var = v;
                ++var;
            }
            var->name = nullptr;
            var->getter = nullptr;
            var->setter = nullptr;

            TypeKey key = s_global->AddTypeInfo(info);
            if (!key.IsValid())
                delete[] reinterpret_cast<char*>(info);
            return key;
        }

    private:
        const char* name_;
        const TypeInfo* super_;
        LuaCastCheck cast_checker_;
        std::vector<TypeFunc> member_func_;
        std::vector<TypeFunc> static_member_func_;
        std::vector<TypeVar> member_var_;
        std::vector<TypeVar> static_var_func_;
    };

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

    GlobalVar::GlobalVar(int version) : version_(version_)
    {
    }

    GlobalVar::~GlobalVar()
    {
    }

    bool GlobalVar::IsValid(const TypeKey& key) const
    {
        return key.serial_ != version_
            && key.index_ >= 0
            && key.index_ < (int)types_.size();
    }

    ITypeDesc* GlobalVar::AllocType(const char* name, const TypeInfo* super)
    {
        return new TypeDesc(name, super);
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

namespace {
    xlua::detail::TypeNode n(&xlua::detail::TypeInfoFunc<TestLuaExport>::GetInfo);
}
const xlua::TypeInfo* xLuaGetTypeInfo(xlua::Identity<TestLuaExport>)
{
    return nullptr;
}

namespace {
    static const xlua::ConstInfo* xLuaGetEnumInfo(xlua::Identity<TestEnum>);
    xlua::detail::ConstNode n2(xLuaGetEnumInfo(xlua::Identity<TestEnum>()));

    //typedef TestEnum EnumType;
    //typedef TestEnum2 EnumType;
    const xlua::ConstInfo* xLuaGetEnumInfo(xlua::Identity<TestEnum>)
    {
        static xlua::ConstInfo info;
        info.name = "";

        static xlua::ConstValue values[] ={
            //xlua::ConstValue("1", 1),
            //xlua::ConstValue("2", 1.0f),
            //xlua::ConstValue("3", "111"),

            //TODO:

            xlua::ConstValue()
        };

        info.values = values;
        return &info;
    }
}

XLUA_EXPORT_ENUM_BEGIN(TestEnum2)
XLUA_EXPORT_ENUM_VAR(kValue11)
XLUA_EXPORT_ENUM_VAR_AS(ttt, kValue12)
XLUA_EXPORT_ENUM_END()
