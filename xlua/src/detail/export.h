#pragma once
#include "../xlua_def.h"
#include "../xlua.h"
#include "traits.h"

XLUA_NAMESPACE_BEGIN

namespace detail
{
    class GlobalVar;
    typedef const TypeInfo* (*fnTypeInfo)();
    typedef const ConstInfo* (*fnConstInfo)();

    enum class NodeType {
        kType,
        kConst,
        kScript,
    };

    struct NodeBase {
        NodeBase(NodeType type);
        ~NodeBase();

        NodeType type_;
        NodeBase* next_;
    };

    struct TypeNode : NodeBase {
        TypeNode(fnTypeInfo func)
            : NodeBase(NodeType::kType)
            , func_(func) {
        }
        const fnTypeInfo func_;
    };

    struct ConstNode : NodeBase {
        ConstNode(fnConstInfo func)
            : NodeBase(NodeType::kConst)
            , func_(func) {
        }
        const fnConstInfo func_;
    };

    struct ScriptNode : NodeBase {
        ScriptNode(const char* script)
            : NodeBase(NodeType::kScript)
            , script_(script) {
        }
        const char* const script_;
    };

    inline ConstValue MakeConstValue() {
        ConstValue cv;
        cv.type = ConstValueType::kNone;
        cv.name = nullptr;
        cv.int_val = 0;
        return cv;
    }

    inline ConstValue MakeConstValue(const char* name, int val) {
        ConstValue cv;
        cv.type = ConstValueType::kInteger;
        cv.name = name;
        cv.int_val = val;
        return cv;
    }

    inline ConstValue MakeConstValue(const char* name, float val) {
        ConstValue cv;
        cv.type = ConstValueType::kFloat;
        cv.name = name;
        cv.float_val = val;
        return cv;
    }

    inline ConstValue MakeConstValue(const char* name, const char* val) {
        ConstValue cv;
        cv.type = ConstValueType::kString;
        cv.name = name;
        cv.string_val = val;
        return cv;
    }

    inline TypeMember MakeMember() {
        TypeMember mem;
        mem.type = MemberType::kInvalid;
        mem.name = nullptr;
        return mem;
    }

    inline TypeMember MakeMember(const char* name, LuaFunction func) {
        TypeMember mem;
        mem.type = MemberType::kFunction;
        mem.name = name;
        mem.func = func;
        return mem;
    }

    inline TypeMember MakeMember(const char* name, LuaIndexer getter, LuaIndexer setter) {
        TypeMember mem;
        mem.type = MemberType::kVariate;
        mem.name = name;
        mem.getter = getter;
        mem.setter = setter;
        return mem;
    }

    /* 导出属性萃取 */
    template <typename GetTy, typename SetTy>
    struct IndexerTrait
    {
    private:
        static constexpr bool all_nullptr = (std::is_null_pointer<GetTy>::value && std::is_null_pointer<SetTy>::value);
        static constexpr bool one_nullptr = (std::is_null_pointer<GetTy>::value || std::is_null_pointer<SetTy>::value);
        static constexpr bool is_get_member = (std::is_member_pointer<GetTy>::value || std::is_member_function_pointer<GetTy>::value);
        static constexpr bool is_set_member = (std::is_member_pointer<SetTy>::value || std::is_member_function_pointer<SetTy>::value);

    public:
        static constexpr bool is_allow = !all_nullptr && (one_nullptr || (is_get_member == is_set_member));
        static constexpr bool is_member = (is_get_member || is_set_member);
    };

    /* 多继承指针偏移（是否要支持多继承？） 
     * add cast mode for this problem
    */
    template <typename Ty, typename By>
    struct CastCheck {
        static void* CastUp(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            if (src == dst)
                return obj;
            return src->super->convert_up(static_cast<By*>((Ty*)obj), src->super, dst);
        }

        static void* CastDown(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            if (src == dst)
                return obj;
            obj = dst->super->convert_down(obj, src, dst->super);
            return obj ? dynamic_cast<Ty*>(static_cast<By*>(obj)) : nullptr;
        }
    };

    template <typename Ty>
    struct CastCheck<Ty, void> {
        static void* CastUp(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            return src == dst ? obj : nullptr;
        }

        static void* CastDown(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            return src == dst ? obj : nullptr;
        }
    };

    template <typename Ty>
    inline int MetaCall(xLuaState* L, Ty f) {
        return 0;
    }

    template <typename Ty>
    inline int MetaGet(xLuaState* L, void* obj, Ty f) {
        return 0;
    }

    template <typename Ty>
    inline int MetaSet(xLuaState* L, void* obj, Ty f) {
        return 0;
    }
} // namespace detail

XLUA_NAMESPACE_END
