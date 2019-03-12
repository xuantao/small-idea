#include "xlua_global.h"
#include "xlua_export.h"

XLUA_NAMESPACE_BEGIN

ITypeDesc* AllocTypeInfo(const char* name, const TypeInfo* super)
{
    return nullptr;
}

const TypeInfo* GetTypeInfo(int index)
{
    return nullptr;
}

namespace detail
{
    static TypeNode* node_head_ = nullptr;
    GlobalVar* GlobalVar::instance_ = nullptr;

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

    TypeNode::TypeNode(GetInfoFunc f) : func(f), next(nullptr)
    {
        next = node_head_;
        node_head_ = this;

        /* 系统已经初始化则注册 */
        //if (GlobalVar::GetInstance())
            //f();
    }

    TypeNode::~TypeNode()
    {
        if (node_head_ == this)
        {
            node_head_ = next;
        }
        else
        {
            TypeNode* node = node_head_;
            while (node)
            {
                if (node->next == this)
                {
                    node->next = next;
                    break;
                }

                node = node->next;
            }
        }

        if (GlobalVar::GetInstance())
        {
            //TODO: 反注册
        }
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
