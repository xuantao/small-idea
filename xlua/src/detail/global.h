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
        xLuaState* GetState(lua_State* l) const;

        ITypeDesc* AllocType(TypeCategory category, bool is_weak_obj, const char* name, const TypeInfo* super);
        const TypeInfo* GetExternalTypeInfo(int index) const;

        int AllocObjIndex(ObjIndex& obj_index, void* obj, const TypeInfo* info);
        int GetSerialNum(int index) const { return 0; }
        ArrayObj* AllocLuaObj(ObjIndex& obj_index, void* obj, const TypeInfo* info);
        ArrayObj* GetArrayObj(int index);

    private:
        void FreeObjIndex(int index);
        void AddTypeInfo(TypeInfo*);
        void* SerialAlloc(size_t size);

    private:
        int version_;
        int serial_num_gener_ = 0;
        std::vector<TypeInfo*> types_;
        std::vector<TypeInfo*> external_types_;
        std::vector<ArrayObj> obj_array_;
        std::vector<int> free_index_;
        std::vector<std::pair<lua_State*, xLuaState*>> states_;
    };
} // namespace detail

XLUA_NAMESPACE_END
