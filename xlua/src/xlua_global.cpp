#include "xlua_global.h"
#include "xlua_export.h"

XLUA_NAMESPACE_BEGIN

namespace detail
{
    static NodeBase* s_node_head = nullptr;
    static GlobalVar* s_global = nullptr;
}

ITypeDesc* AllocTypeInfo(const char* name, const TypeInfo* super)
{
    return nullptr;
}

const TypeInfo* GetTypeInfo(int index)
{
    return nullptr;
}

ObjIndex::~ObjIndex()
{
    if (index_ != -1 && detail::s_global)
        ;   //TODO: release
}

namespace detail
{
    struct TypeDesc : public ITypeDesc
    {
    public:
        TypeDesc(const char* name, const TypeInfo* super) : name_(name), super_(super) { }
        ~TypeDesc() { }

        void AddFunc(const char* name, LuaFunction func, bool global) override
        {
            MemberFunc mf{ name, func };
            if (global)
                static_member_func_.push_back(mf);
            else
                member_func_.push_back(mf);
        }

        void AddVar(const char* name, LuaIndexer getter, LuaIndexer setter, bool global) override
        {
            MemberVar mv{ name, getter, setter };
            if (global)
                static_var_func_.push_back(mv);
            else
                member_var_.push_back(mv);
        }

        int Finalize() override
        {
            if (GlobalVar::GetInstance() == nullptr)
                return -1;
            return  GlobalVar::GetInstance()->AddTypeInfo(this);
        }

        GlobalVar* global_;
        const char* name_;
        const TypeInfo* super_;
        std::vector<MemberFunc> member_func_;
        std::vector<MemberFunc> static_member_func_;
        std::vector<MemberVar> member_var_;
        std::vector<MemberVar> static_var_func_;
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

        s_global = new GlobalVar();
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

    GlobalVar::GlobalVar()
    {

    }

    GlobalVar::~GlobalVar()
    {

    }

    ITypeDesc* GlobalVar::AllocType(const char* name, const TypeInfo* super)
    {
        return nullptr;
    }

    const TypeInfo* GlobalVar::GetTypeInfo(int index) const
    {
        return nullptr;
    }

    int GlobalVar::AddTypeInfo(TypeDesc* desc)
    {
        return -1;
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

};

enum class TestEnum2
{

};

struct TestLuaExport
{
    int ia;
    void test_call() {}
};

namespace {
    xlua::detail::TypeNode n(&xlua::detail::TypeInfoFunc<TestLuaExport>::GetInfo);
}
const xlua::TypeInfo* xLuaGetTypeInfo(xlua::Indetity<TestLuaExport>)
{
    return nullptr;
}

namespace {
    static const xlua::ConstInfo* xLuaGetEnumInfo(xlua::Indetity<TestEnum>);
    xlua::detail::ConstNode n2(xLuaGetEnumInfo(xlua::Indetity<TestEnum>()));

    typedef TestEnum EnumType;
    typedef TestEnum2 EnumType;
    const xlua::ConstInfo* xLuaGetEnumInfo(xlua::Indetity<TestEnum>)
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
