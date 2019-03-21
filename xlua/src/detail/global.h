#pragma once
#include "../xlua_def.h"
#include <vector>

XLUA_NAMESPACE_BEGIN

namespace detail
{
    struct TypeDesc;
    struct ArrayObj {
        int serial_num_;
        void* obj_;
        const TypeInfo* info_;
    };

    class GlobalVar
    {
        friend struct TypeDesc;
        friend class ObjIndex;
    private:
        GlobalVar(int version);
        ~GlobalVar();

    public:
        static bool Startup();
        static GlobalVar* GetInstance();

        void Purge();

    public:
        xLuaState* Create();
        xLuaState* Attach(lua_State* l);
        void Destory(xLuaState* l);

        bool IsValid(const TypeKey& key) const;
        ITypeDesc* AllocType(TypeCategory category, bool is_weak_obj, const char* name, const TypeInfo* super);
        const TypeInfo* GetTypeInfo(const TypeKey& key) const;
        const TypeInfo* GetExternalInfo(int index) const { return nullptr; }

        int AllocObjIndex(ObjIndex& obj_index, void* obj, const TypeInfo* info);
        ArrayObj* GetArrayObj(int index);
        int GetObjSerialNum(int index) {
            ArrayObj* obj = GetArrayObj(index);
            return obj ? obj->serial_num_ : 0;
        }

    private:
        TypeKey AddTypeInfo(TypeInfo* info);
        void FreeObjIndex(int index);

    private:
        int version_;
        int serial_num_gener_ = 0;
        std::vector<TypeInfo*> types_;
        std::vector<ArrayObj> obj_array_;
        std::vector<int> free_index_;
    };
} // namespace detail

XLUA_NAMESPACE_END
