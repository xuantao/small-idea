#include "xlua_global.h"

XLUA_NAMESPACE_BEGIN

namespace xlua_internal
{
    static TypeInitNode* s_head = nullptr;
    static GlobalVar* s_global = nullptr;

    TypeInitNode::TypeInitNode(TypeRegisterFunc f) : func(f), next(nullptr)
    {
        next = s_head;
        s_head = this;

        if (s_global)
        {
            //TODO: ×¢²á
        }
    }

    TypeInitNode::~TypeInitNode()
    {
        if (s_head == this)
            s_head = s_head->next;

        TypeInitNode* node = s_head;
        while (node)
        {
            if (node->next == this)
            {
                node->next = next;
                break;
            }
            else
            {
                node = node->next;
            }
        }

        if (s_global)
        {
            //TODO: ·´×¢²á
        }
    }

} // namespace xlua_internal

XLUA_NAMESPACE_END
