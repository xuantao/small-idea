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

    /* 多继承指针偏移, 这里一步步的执行转换 */
    template <typename Ty, typename By>
    struct PtrCast {
        static void* CastUp(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            if (src == dst)
                return obj;
            return src->super->converter.convert_up(static_cast<By*>((Ty*)obj), src->super, dst);
        }

        static void* CastDown(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            if (src == dst)
                return obj;
            obj = dst->super->converter.convert_down(obj, src, dst->super);
            return obj ? dynamic_cast<Ty*>(static_cast<By*>(obj)) : nullptr;
        }
    };

    template <typename Ty>
    struct PtrCast<Ty, void> {
        static void* CastUp(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            return src == dst ? obj : nullptr;
        }

        static void* CastDown(void* obj, const TypeInfo* src, const TypeInfo* dst) {
            return src == dst ? obj : nullptr;
        }
    };

    template <typename Ty, typename By>
    struct SharedPtrCast {
        static bool Cast(void* src, const TypeInfo* src_type, void* dst, const TypeInfo* dst_type) {
            if (src_type == dst_type) {
                *(std::shared_ptr<Ty>*)dst = *(std::shared_ptr<Ty>*)src;
                return true;
            }

            std::shared_ptr<By> tmp = std::static_pointer_cast<By>(*(std::shared_ptr<Ty>*)src);
            return src_type->super->converter.convert_shared_ptr(&tmp, src_type->super, dst, dst_type);
        }
    };

    template <typename Ty>
    struct SharedPtrCast<Ty, void> {
        static bool Cast(void* src, const TypeInfo* src_type, void* dst, const TypeInfo* dst_type) {
            if (src_type != dst_type)
                return false;
            *(std::shared_ptr<Ty>*)dst = *(std::shared_ptr<Ty>*)src;
            return true;
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
