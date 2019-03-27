#include "xlua.h"
#include "detail/global.h"

XLUA_NAMESPACE_BEGIN

#define _XLUA_GET_STATE(l)  xlua::detail::GlobalVar::GetInstance()->GetState(l)

namespace detail {
    struct LightPtrInfo {
        void* obj;
        const TypeInfo* info;
    };

    static void TraceCallStack(lua_State* L, char* buf, size_t size) {
        char* dst = buf;
        int len = (int)size;
        int level = 1;
        lua_Debug dbg;

        while (len > 0 && lua_getstack(L, level, &dbg)) {
            lua_getinfo(L, "Sl", &dbg);
            int l = snprintf(dst, len, "%s:%d\n", dbg.source, dbg.currentline);
            dst += l;
            len -= l;

            ++level;
        }
    };

    static void LogCallStack(lua_State* l) {
        char buf[1024];
        TraceCallStack(l, buf, 1024);
        printf("%s", buf);
    }

    static bool GetLightPtrMetaMember(lua_State* l, LightPtrInfo& ret) {
        LightDataPtr ptr = MakeLightPtr(lua_touserdata(l, 1));
        if (ptr.ptr_ == nullptr) {
            // log err
            return false;
        }

        if (ptr.type_ == 0) {
            ArrayObj* ary_obj = GlobalVar::GetInstance()->GetArrayObj(ptr.index_);
            if (ary_obj->serial_num_ != ptr.serial_) {
                // log err
                return false;
            }

            ret.info = ary_obj->info_;
            ret.obj = ary_obj->obj_;
        } else {
            ret.info = GlobalVar::GetInstance()->GetExternalTypeInfo(ptr.type_);
            if (ret.info->is_weak_obj) {
                if (ptr.serial_ != xLuaGetWeakObjSerialNum(ptr.index_)) {
                    // log err
                    return false;
                }

                ret.obj = xLuaGetWeakObjPtr(ptr.index_);
            } else {
                ret.obj = ptr.ToRawPtr();
            }
        }

        return true;
    }

    struct MetaFuncs {
        static int LuaIndex(lua_State* l) {
            if (0 == lua_getmetatable(l, 1)) {
                // log err
                return 0;
            }

            lua_pushvalue(l, -2);
            int ty = lua_rawget(l, -2);
            if (ty == LUA_TFUNCTION)
                return 1;

            if (ty != LUA_TLIGHTUSERDATA) {
                // log err
                return 0;
            }

            TypeMember* mem = (TypeMember*)lua_touserdata(l, -1);
            assert(mem->type == MemberType::kVariate);

            if (mem->getter == nullptr) {
                // log err
                return 0;
            }

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            LuaUserData* ud = (LuaUserData*)lua_touserdata(l, 1);
            mem->getter(xl, ud->obj_);
            return 1;
        }

        static int LuaNewIndex(lua_State* l) {
            if (0 == lua_getmetatable(l, 1)) {
                // log err
                return 0;
            }

            lua_pushvalue(l, -2);           // copy key to top
            int ty = lua_rawget(l, -2);
            if (ty != LUA_TLIGHTUSERDATA) {
                return 0;
            }

            TypeMember* mem = (TypeMember*)lua_touserdata(l, -1);
            assert(mem->type == MemberType::kVariate);
            if (mem->setter == nullptr) {
                // log err
                return 0;
            }

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            LuaUserData* ud = (LuaUserData*)lua_touserdata(l, 1);
            mem->setter(xl, ud->obj_);
            return 0;
        }

        static int LuaGc(lua_State* l) {
            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            xl->Gc(static_cast<LuaUserData*>(lua_touserdata(l, 1)));
            return 0;
        }

        static int LuaToString(lua_State* l) {
            return 0;
        }

        static int LuaLen(lua_State* l) {
            return 0;
        }

        static int LuaPairs(lua_State* l) {
            return 0;
        }

        static int LuaLightPtrIndex(lua_State* l) {
            LightPtrInfo ptr;
            if (!GetLightPtrMetaMember(l, ptr))
                return 0;

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            lua_rawgeti(l, LUA_REGISTRYINDEX, xl->meta_table_ref_);
            lua_rawgeti(l, -1, ptr.info->index);
            lua_pushvalue(l, -3);
            int ty = lua_rawget(l, -2);
            if (ty == LUA_TFUNCTION)
                return 1;   // function

            if (ty != LUA_TLIGHTUSERDATA) {
                // log err
                return 0;
            }

            TypeMember* mem = (TypeMember*)lua_touserdata(l, -1);
            assert(mem->type == MemberType::kVariate);
            if (mem->getter == nullptr) {
                // log err
                return 0;
            }

            mem->getter(xl, ptr.obj);
            return 1;
        }

        static int LuaLightPtrNewIndex(lua_State* l) {
            LightPtrInfo ptr;
            if (!GetLightPtrMetaMember(l, ptr))
                return 0;

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            lua_rawgeti(l, LUA_REGISTRYINDEX, xl->meta_table_ref_);
            lua_rawgeti(l, -1, ptr.info->index);
            lua_pushvalue(l, -3);
            if (lua_rawget(l, -2) != LUA_TLIGHTUSERDATA) {
                // log err
                return 0;
            }

            TypeMember* mem = (TypeMember*)lua_touserdata(l, -1);
            assert(mem->type == MemberType::kVariate);
            if (mem->setter == nullptr) {
                // log err
                return 0;
            }

            mem->setter(xl, ptr.obj);
            return 0;
        }

        static int LuaGlobalIndex(lua_State* l) {
            return 0;
        }

        static int LuaGlobalNewIndex(lua_State* l) {
            return 0;
        }

        static int LuaConstIndex(lua_State* l) {
            //const char* pszKey = lua_tostring(L, 2);
            //if (pszKey)
            {
                //TraceCallStack(L);
            }
            return 0;
        }

        static int LuaConstNewIndex(lua_State* l) {
            //const char* pszKey = lua_tostring(L, 2);
            //if (pszKey)
            {
                //TraceCallStack(L);
            }
            return 0;
        }
    };
}

bool Startup() {
    return detail::GlobalVar::Startup();
}

void Shutdown() {
    auto global = detail::GlobalVar::GetInstance();
    if (global == nullptr)
        return;

    global->Purge();
}

xLuaState* Create() {
    auto global = detail::GlobalVar::GetInstance();
    if (global == nullptr)
        return nullptr;

    return global->Create();
}

xLuaState* Attach(lua_State* l) {
    auto global = detail::GlobalVar::GetInstance();
    if (global == nullptr)
        return nullptr;

    return global->Attach(l);
}

xLuaState::xLuaState(lua_State* l, bool attach)
    : state_(l)
    , attach_(attach) {
}

xLuaState::~xLuaState() {

}

void xLuaState::Release() {
    auto global = detail::GlobalVar::GetInstance();
    assert(global == nullptr);
    global->Destory(this);
}

void xLuaState::LogCallStack() const {
    detail::LogCallStack(state_);
}

void xLuaState::GetCallStack(char* buf, size_t size) const {
    detail::TraceCallStack(state_, buf, size);
}

const char* xLuaState::GetTypeName(int index) {
    *type_name_buf_ = 0;
    int l_ty = GetType(index);
    const char* ret = type_name_buf_;

    if (l_ty == LUA_TLIGHTUSERDATA) {
#if XLUA_USE_LIGHT_USER_DATA
        detail::LightDataPtr ptr = detail::MakeLightPtr(lua_touserdata(state_, index));
        if (ptr.type_ == 0) {
            detail::ArrayObj* obj = detail::GlobalVar::GetInstance()->GetArrayObj(ptr.index_);
            if (obj == nullptr)
                snprintf(type_name_buf_, 256, "nullptr");
            else
                snprintf(type_name_buf_, 256, "%s*", obj->info_->type_name);
        }
#endif // XLUA_USE_LIGHT_USER_DATA
    } else if (l_ty == LUA_TUSERDATA) {
        detail::LuaUserData* ud = static_cast<detail::LuaUserData*>(lua_touserdata(state_, index));
        switch (ud->type_)
        {
        case detail::LuaUserDataType::kValue:
            ret = ud->info_->type_name;
            break;
        case detail::LuaUserDataType::kRawPtr:
        case detail::LuaUserDataType::kObjPtr:
            snprintf(type_name_buf_, 256, "%s*", ud->info_->type_name);
            break;
        case detail::LuaUserDataType::kSharedPtr:
            snprintf(type_name_buf_, 256, "std::shared_ptr<%s>", ud->info_->type_name);
            break;
        default:
            assert(false);
            break;
        }
    } else {
        ret = lua_typename(state_, index);
    }
    return ret;
}

bool xLuaState::DoString(const char* stream, const char* chunk) {
    return true;
}

int xLuaState::LoadGlobal(const char* path) {
    return 0;
}

void xLuaState::SetGlobal(const char* path) {

}

int xLuaState::LoadTableField(int index, int field) {
    return 0;
}

int xLuaState::LoadTableField(int index, const char* field) {
    return 0;
}

void xLuaState::SetTableField(int index, int field) {

}

void xLuaState::SetTableField(int index, const char* field) {

}

bool xLuaState::_TryPushSharedPtr(void* root, void* ptr, const TypeInfo* info) {
    auto it = shared_ptrs_.find(root);
    if (it == shared_ptrs_.cend())
        return false;

    //PushCacheUd(it->second, ptr, info);
    return true;
}

void xLuaState::_PushSharedPtr(void* root, detail::LuaUserData* user_data) {
    auto& cache = shared_ptrs_[root];
    cache.user_data_ = user_data;
    PushUd(cache);
}

void xLuaState::_PushUniquePtr(detail::LuaUserData* user_data) {
    printf("push unique_ptr\n");
    //TODO: real push to lua
}

void xLuaState::_PushValue(detail::LuaUserData* user_data) {

}

bool xLuaState::_TryPushRawPtr(void* root, void* ptr, const TypeInfo* info) {
    auto it = raw_ptrs_.find(root);
    if (it == raw_ptrs_.cend())
        return false;

    //PushCacheUd(it->second, ptr, info);
    return true;
}

void xLuaState::_PushRawPtr(void* root, detail::LuaUserData* user_data) {
    auto& cache = raw_ptrs_[root];
    cache.user_data_ = user_data;
    PushUd(cache);
}

void xLuaState::Gc(detail::LuaUserData* user_data) {

}

bool xLuaState::InitEnv(const std::vector<TypeInfo*>& types,
    const std::vector<const ConstInfo*>& consts,
    const std::vector<const char*>& scripts) {
    // user data ref table (weak table)
    lua_createtable(state_, 1024, 0);
    lua_createtable(state_, 0, 1);
    lua_pushfstring(state_, "v");
    lua_setfield(state_, 2, "__mode");
    lua_setmetatable(state_, -2);
    user_data_table_ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
    assert(user_data_table_ref_);
    assert(lua_gettop(state_) == 0);

    // table of type metatable
    lua_createtable(state_, (int)types.size(), 0);
    meta_table_ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
    assert(meta_table_ref_);

    lua_newtable(state_);
    lua_obj_table_ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
    assert(lua_obj_table_ref_);

#if XLUA_USE_LIGHT_USER_DATA
    // light user data metatable
    lua_pushlightuserdata(state_, nullptr);  //修改lightuserdata的metatable
    lua_createtable(state_, 0, 5);

    lua_pushstring(state_, "light_user_data_metatable");
    lua_setfield(state_, -2, "__name");

    PushClosure(&detail::MetaFuncs::LuaLightPtrIndex);
    lua_setfield(state_, -2, "__index");

    PushClosure(&detail::MetaFuncs::LuaLightPtrNewIndex);
    lua_setfield(state_, -2, "__newindex");

    lua_setmetatable(state_, -2);   // set light user data metatable
    lua_pop(state_, 1);             // light user data
    assert(lua_gettop(state_) == 0);
#endif // XLUA_USE_LIGHT_USER_DATA

    for (const TypeInfo* info : types) {
        if (info)
            CreateMeta(info);
    }

    AddConsts(consts);

    for (const char* script : scripts) {
        DoString(script, "InitEnv");
    }

    return true;
}

void xLuaState::AddConsts(const std::vector<const ConstInfo*>& consts) {
    lua_createtable(state_, 2, 0);  // const metatable

    lua_pushcfunction(state_, &detail::MetaFuncs::LuaConstIndex);
    lua_setfield(state_, -2, "__index");
    lua_pushcfunction(state_, &detail::MetaFuncs::LuaConstNewIndex);
    lua_setfield(state_, -2, "__newindex");

    for (const ConstInfo* info : consts) {
        int num = 0;
        while (info->values[num].name)
            ++num;

        const char* name = info->name;
        lua_pushglobaltable(state_);
        while (const char* sub = ::strchr(name, '.')) {
            lua_pushlstring(state_, name, sub - name);
            lua_gettable(state_, -2);
            if (lua_isnil(state_, -1)) {
                lua_pushlstring(state_, name, sub - name);
                lua_newtable(state_);
                lua_settable(state_, -3);

                lua_pushlstring(state_, name, sub - name);
                lua_gettable(state_, -2);
                lua_remove(state_, -2);
            }

            assert(lua_type(state_, -1) == LUA_TTABLE);
            name = sub + 1;
        }

        lua_pushstring(state_, name);
        lua_createtable(state_, num, 0);

        for (int i = 0; i < num; ++i) {
            const auto& val = info->values[i];
            switch (val.type) {
            case ConstValueType::kInteger:
                lua_pushnumber(state_, val.int_val);
                break;
            case ConstValueType::kFloat:
                lua_pushnumber(state_, val.float_val);
                break;
            case ConstValueType::kString:
                lua_pushstring(state_, val.string_val);
                break;
            default:
                assert(false);
                break;
            }

            lua_setfield(state_, -2, val.name);
        }

        lua_pushvalue(state_, -3);          // copy meta table to top stack
        lua_setmetatable(state_, -2);       // set metatable
        lua_settable(state_, -3);           // set global
        lua_pop(state_, 1);                 // pop global table
    }

    lua_pop(state_, 1); // pop const meta table
    assert(lua_gettop(state_) == 0);
}

void xLuaState::CreateMeta(const TypeInfo* info) {
    int member_num = 0;
    int global_num = 0;
    while (info->members[member_num].type != MemberType::kInvalid)
        ++member_num;
    while (info->globals[global_num].type != MemberType::kInvalid)
        ++global_num;

    if (member_num > 0) {
        lua_rawgeti(state_, LUA_REGISTRYINDEX, meta_table_ref_);
        lua_createtable(state_, 0, member_num + 4);
        lua_rawseti(state_, -2, info->index);
        lua_rawgeti(state_, -1, info->index);
        lua_remove(state_, -2);

        for (const TypeMember* mem = info->members; mem->type!= MemberType::kInvalid; ++mem) {
            switch (mem->type) {
            case MemberType::kFunction:
                //lua_pushcfunction(state_, mem->func);
                PushClosure(mem->func);
                break;
            case MemberType::kVariate:
                lua_pushlightuserdata(state_, const_cast<TypeMember*>(mem));
                break;
            }
            lua_setfield(state_, -2, mem->name);
        }

        lua_pushstring(state_, info->type_name);
        lua_setfield(state_, -2, "__name");

        PushClosure(&detail::MetaFuncs::LuaIndex);
        lua_setfield(state_, -2, "__index");

        PushClosure(&detail::MetaFuncs::LuaNewIndex);
        lua_setfield(state_, -2, "__newindex");

        PushClosure(&detail::MetaFuncs::LuaGc);
        lua_setfield(state_, -2, "__gc");

        lua_pop(state_, 1);
        assert(lua_gettop(state_) == 0);
    }

    if (global_num > 0) {
        const char* name = info->type_name;
        lua_pushglobaltable(state_);
        while (const char* sub = ::strchr(name, '.')) {
            lua_pushlstring(state_, name, sub - name);
            lua_gettable(state_, -2);
            if (lua_isnil(state_, -1)) {
                lua_pushlstring(state_, name, sub - name);
                lua_newtable(state_);
                lua_settable(state_, -3);

                lua_pushlstring(state_, name, sub - name);
                lua_gettable(state_, -2);
                lua_remove(state_, -2);
            }

            assert(lua_type(state_, -1) == LUA_TTABLE);
            name = sub + 1;
        }
        lua_pushstring(state_, name);

        lua_createtable(state_, 0, 0);
        for (const TypeMember* mem = info->members; mem->type!= MemberType::kInvalid; ++mem) {
            if (mem->type == MemberType::kFunction) {
                //lua_pushcfunction(state_, mem->func);
                PushClosure(mem->func);
                lua_setfield(state_, -2, mem->name);
            }
        }

        // meta table
        lua_createtable(state_, 0, 0);
        for (const TypeMember* mem = info->members; mem->type!= MemberType::kInvalid; ++mem) {
            if (mem->type == MemberType::kVariate) {
                lua_pushlightuserdata(state_, const_cast<TypeMember*>(mem));
                lua_setfield(state_, -2, mem->name);
            }
        }

        lua_pushstring(state_, info->type_name);
        lua_setfield(state_, -2, "__name");

        PushClosure(&detail::MetaFuncs::LuaGlobalIndex);
        lua_setfield(state_, -2, "__index");

        PushClosure(&detail::MetaFuncs::LuaGlobalNewIndex);
        lua_setfield(state_, -2, "__newindex");

        lua_setmetatable(state_, -2);

        lua_settable(state_, -3);
        lua_pop(state_, 1);

        assert(lua_gettop(state_) == 0);
    }
}

void xLuaState::PushClosure(lua_CFunction func) {
    lua_pushlightuserdata(state_, this);                        // upvalue(1):xLuaSate*
    //lua_rawgeti(state_, LUA_REGISTRYINDEX, meta_table_ref_);    // upvalue(2):meta table
    lua_pushcclosure(state_, func, 2);
}

int xLuaState::RefLuaObj(int index) {
    if (next_free_lua_obj_ == -1) {
        size_t o_s = lua_objs_.size();
        size_t n_s = o_s + 1024;
        lua_objs_.resize(n_s, LuaObjRef{-1, 0});

        for (size_t i = o_s; i < n_s; ++i)
            lua_objs_[i].next_free_ = i+1;
        next_free_lua_obj_ = o_s;
        lua_objs_.back().next_free_ = -1;
    }

    int ary_index = next_free_lua_obj_;
    auto& ref = lua_objs_[ary_index];
    next_free_lua_obj_ = ref.next_free_;

    index = lua_absindex(state_, index);
    lua_rawgeti(state_, LUA_REGISTRYINDEX, lua_obj_table_ref_);
    lua_pushvalue(state_, index);
    ref.lua_ref_ = luaL_ref(state_, -2);
    ref.ref_count_ = 1;
    lua_pop(state_, 1);

    return ary_index;
}

void xLuaState::PushLuaObj(int ary_index) {
    assert(ary_index >= 0 && ary_index < (int)lua_objs_.size());

    lua_rawgeti(state_, LUA_REGISTRYINDEX, lua_obj_table_ref_);
    lua_rawgeti(state_, -1, lua_objs_[ary_index].lua_ref_);
    lua_remove(state_, -2);
}

void xLuaState::AddObjRef(int ary_index) {
    assert(ary_index >= 0 && ary_index < (int)lua_objs_.size());
    ++lua_objs_[ary_index].ref_count_;
}

void xLuaState::UnRefObj(int ary_index) {
    assert(ary_index >= 0 && ary_index < (int)lua_objs_.size());
    auto& ref = lua_objs_[ary_index];
    -- ref.ref_count_;
    if (ref.ref_count_ == 0) {
        lua_rawgeti(state_, LUA_REGISTRYINDEX, lua_obj_table_ref_);
        luaL_unref(state_, -1, ref.lua_ref_);
        lua_pop(state_, 1);

        int tmp = ref.next_free_;
        ref.next_free_ = next_free_lua_obj_;
        next_free_lua_obj_ = tmp;
    }
}

#if XLUA_USE_LIGHT_USER_DATA
#else // XLUA_USE_LIGHT_USER_DATA
void xLuaState::_PushWeakObjPtr(const detail::LuaUserData user_data) {

}
#endif // XLUA_USE_LIGHT_USER_DATA

XLUA_NAMESPACE_END
