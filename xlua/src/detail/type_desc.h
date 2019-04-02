#pragma once
#include "../xlua_def.h"
#include <vector>

XLUA_NAMESPACE_BEGIN

namespace detail
{
    struct TypeDesc : public ITypeDesc
    {
        friend class GlobalVar;
    public:
        TypeDesc(GlobalVar* mgr, TypeCategory category, bool is_wak_obj, const char* name, const TypeInfo* super)
            : mgr_(mgr)
            , category_(category)
            , is_weak_obj_(is_wak_obj)
            , name_(name)
            , super_(super) {
        }

        virtual ~TypeDesc() { }

    public:
        static void Free(TypeInfo* info);

    public:
        void SetCaster(TypeCaster caster) override {
            caster_ = caster;
        }

        void AddMember(const char* name, LuaFunction func, bool global) override;
        void AddMember(const char* name, LuaIndexer getter, LuaIndexer setter, bool global) override;
        const TypeInfo* Finalize() override;

    private:
        void PerifyTypeName(TypeInfo* info);

    private:
        GlobalVar* mgr_;
        TypeCategory category_;
        bool is_weak_obj_;
        const char* name_;
        const TypeInfo* super_;
        TypeCaster caster_;
        std::vector<MemberVar> vars_;
        std::vector<MemberFunc> funcs_;
        std::vector<MemberVar> global_vars_;
        std::vector<MemberFunc> global_funcs_;
    };
} // namespace detail

XLUA_NAMESPACE_END
