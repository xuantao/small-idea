#include "xlua_global.h"

XLUA_NAMESPACE_BEGIN

namespace xlua_internal
{
    static TypeNode* s_head = nullptr;
    static GlobalVar* s_global = nullptr;

    TypeNode::TypeNode(GetTypeInfoFunc f) : func(f), next(nullptr)
    {
        next = s_head;
        s_head = this;

        if (s_global)
        {
            //TODO: ×¢²á
        }
    }

    TypeNode::~TypeNode()
    {
        if (s_head == this)
            s_head = s_head->next;

        TypeNode* node = s_head;
        while (node)
        {
            if (node->next == this)
            {
                node->next = next;
                break;
            }

            node = node->next;
        }

        if (s_global)
        {
            //TODO: ·´×¢²á
        }
    }

} // namespace xlua_internal

namespace
{
    static int LuaExpType_Index = -1;
    struct LuaExpType_Node : private xlua_internal::TypeNode
    {
        LuaExpType_Node() : xlua_internal::TypeNode(&LuaExpType::LuaGetTypeInfo) { }
        ~LuaExpType_Node() { LuaExpType_Index = -1; }
    } LuaExpType_Node_;
}

const TypeInfo* LuaExpType::LuaGetTypeInfo()
{
    //TODO:
    if (LuaExpType_Index != -1)
        return xlua_internal::GetTypeInfo(LuaExpType_Index);

    ITypeDesc* desc = xlua_internal::AllocTypeInfo();
    if (desc == nullptr) return nullptr;

    //TODO:
    desc->SetName("");
    desc->SetSupper(nullptr);
    desc->AddFunc("", nullptr, false);

    LuaExpType_Index = desc->Finalize();
    return xlua_internal::GetTypeInfo(LuaExpType_Index);
}

XLUA_NAMESPACE_END
