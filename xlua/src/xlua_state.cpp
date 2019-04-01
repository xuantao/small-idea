#include "xlua_state.h"
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
        char buf[XLUA_MAX_BUFFER_CACHE];
        TraceCallStack(l, buf, XLUA_MAX_BUFFER_CACHE);
        xLuaLogError(buf);
    }

    static bool HasGlobal(const TypeInfo* info) {
        while (info) {
            if (info->globals[0].category != MemberCategory::kInvalid)
                return true;
            info = info->super;
        }
        return false;
    }

    struct MetaFuncs {
        static int LuaIndex(lua_State* l) {
            lua_getmetatable(l, 1);     // 1: ud, 2: key, 3: metatable
            lua_pushvalue(l, 2);        // 4: copy key
            int ty = lua_rawget(l, -2); // 4: table member
            if (ty == LUA_TFUNCTION)
                return 1;

            FullUserData* ud = (FullUserData*)lua_touserdata(l, 1);
            if (ty != LUA_TLIGHTUSERDATA) {
                detail::LogError("type:[%s] member:[%s] is not exist", ud->info_->type_name, lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            TypeMember* mem = (TypeMember*)lua_touserdata(l, -1);
            assert(mem->category == MemberCategory::kVariate);
            if (mem->getter == nullptr) {
                detail::LogError("type:[%s] member:[%s] can not been read", ud->info_->type_name, lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            mem->getter(xl, ud->obj_, ud->info_);
            return 1;
        }

        static int LuaNewIndex(lua_State* l) {
            FullUserData* ud = (FullUserData*)lua_touserdata(l, 1);
            lua_getmetatable(l, 1);     // 1: ud, 2: key, 3: value, 4: metatable
            lua_pushvalue(l, 2);        // 5: copy_key
            int ty = lua_rawget(l, -2); // -1: key, -2: metatable
            if (ty != LUA_TLIGHTUSERDATA) {
                detail::LogError("type:[%s] member:[%s] is not exist", ud->info_->type_name, lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            TypeMember* mem = (TypeMember*)lua_touserdata(l, -1);
            assert(mem->category == MemberCategory::kVariate);
            if (mem->setter == nullptr) {
                detail::LogError("type:[%s] member:[%s] is read only", ud->info_->type_name, lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            lua_pop(l, 2);  // pop: -1: type_member, -2: metatable
            mem->setter(xl, ud->obj_, ud->info_);
            return 0;
        }

        static int LuaGc(lua_State* l) {
            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            xl->Gc(static_cast<FullUserData*>(lua_touserdata(l, 1)));
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

#if XLUA_USE_LIGHT_USER_DATA
        static LightPtrInfo GetLightPtrInfo(lua_State* l) {
            LightPtrInfo ret{ nullptr, nullptr };
            LightUserData ptr = MakeLightPtr(lua_touserdata(l, 1));
            if (ptr.ptr_ == nullptr)
                return ret;

            if (ptr.type_ == 0) {
                ArrayObj* ary_obj = GlobalVar::GetInstance()->GetArrayObj(ptr.index_);
                if (ary_obj->serial_num_ != ptr.serial_)
                    return ret;

                ret.info = ary_obj->info_;
                ret.obj = ary_obj->obj_;
            } else {
                const TypeInfo* info = GlobalVar::GetInstance()->GetExternalTypeInfo(ptr.type_);
                if (info->is_weak_obj) {
                    if (ptr.serial_ != xLuaGetWeakObjSerialNum(ptr.index_))
                        return ret;

                    ret.obj = info->caster.to_weak_ptr(xLuaGetWeakObjPtr(ptr.index_));
                } else {
                    ret.obj = ptr.ToRawPtr();
                }

                ret.info = info;
            }

            return ret;
        }

        // stack info, 1: ud, 2: key
        static int LuaLightPtrIndex(lua_State* l) {
            LightPtrInfo ptr_info = GetLightPtrInfo(l);
            if (ptr_info.obj == nullptr) {
                detail::LogError("light user data ptr is null");
                LogCallStack(l);
                return 0;
            }

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            lua_rawgeti(l, LUA_REGISTRYINDEX, xl->meta_table_ref_); // metatable list
            lua_rawgeti(l, -1, ptr_info.info->index);               // active metatable
            lua_pushvalue(l, 2);            // copy key
            int ty = lua_rawget(l, -2);     // 1: ud, 2: key, 3: meta_list, 4: metatable, 5: key
            if (ty == LUA_TFUNCTION)
                return 1;   // function

            if (ty != LUA_TLIGHTUSERDATA) {
                detail::LogError("type:[%s] member:[%s] is not exist", ptr_info.info->type_name, lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            TypeMember* mem = (TypeMember*)lua_touserdata(l, -1);
            assert(mem->category == MemberCategory::kVariate);
            if (mem->getter == nullptr) {
                detail::LogError("type:[%s] member:[%s] can not been read", ptr_info.info->type_name, lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            mem->getter(xl, ptr_info.obj, ptr_info.info);
            return 1;
        }

        // lua stack, [1]:ud, [2]:key, [3]:value
        static int LuaLightPtrNewIndex(lua_State* l) {
            LightPtrInfo ptr_info = GetLightPtrInfo(l);
            if (ptr_info.obj == nullptr) {
                detail::LogError("light user data ptr is null");
                LogCallStack(l);
                return 0;
            }

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            lua_rawgeti(l, LUA_REGISTRYINDEX, xl->meta_table_ref_);
            lua_rawgeti(l, -1, ptr_info.info->index);
            lua_pushvalue(l, 2);    // copy key to top
            if (lua_rawget(l, -2) != LUA_TLIGHTUSERDATA) {
                detail::LogError("type:[%s] member:[%s] can not been modify", ptr_info.info->type_name, lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            TypeMember* mem = (TypeMember*)lua_touserdata(l, -1);
            assert(mem->category == MemberCategory::kVariate);
            if (mem->setter == nullptr) {
                detail::LogError("type:[%s] member:[%s] is read only", ptr_info.info->type_name, lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            // stack: [1]:ud, [2]:key, [3]:value, [4]:meta_list, [5]:metatable, [6]: member
            lua_pop(l, 3);

            mem->setter(xl, ptr_info.obj, ptr_info.info);
            return 0;
        }
#endif
        static int LuaGlobalIndex(lua_State* l) {
            lua_getmetatable(l, 1);
            lua_pushvalue(l, 2);
            int ty = lua_rawget(l, -2); // 4: table member
            if (ty != LUA_TLIGHTUSERDATA) {
                lua_pushstring(l, "__name");
                detail::LogError("type:[%s] member:[%s] is not exist", lua_rawget(l, 3), lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            TypeMember* mem = (TypeMember*)lua_touserdata(l, -1);
            assert(mem->category == MemberCategory::kVariate);
            if (mem->getter == nullptr) {
                lua_pushstring(l, "__name");
                detail::LogError("type:[%s] member:[%s] can not been read", lua_rawget(l, 3), lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            mem->getter(xl, nullptr, nullptr);
            return 1;
        }

        static int LuaGlobalNewIndex(lua_State* l) {
            lua_getmetatable(l, 1);
            lua_pushvalue(l, 2);
            int ty = lua_rawget(l, -2); // 5: table member
            if (ty != LUA_TLIGHTUSERDATA) {
                lua_pushstring(l, "__name");
                detail::LogError("type:[%s] member:[%s] is not exist", lua_rawget(l, 4), lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            TypeMember* mem = (TypeMember*)lua_touserdata(l, -1);
            assert(mem->category == MemberCategory::kVariate);
            if (mem->setter == nullptr) {
                lua_pushstring(l, "__name");
                detail::LogError("type:[%s] member:[%s] can not been write", lua_rawget(l, 4), lua_tostring(l, 2));
                LogCallStack(l);
                return 0;
            }

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            mem->setter(xl, nullptr, nullptr);
            return 0;
        }

        static int LuaConstIndex(lua_State* l) {
            lua_pushstring(l, "__name");
            if (lua_rawget(l, 1) != LUA_TSTRING)
                return 0;

            detail::LogError("const Type:[%s] member:[%s] does not exist", lua_tostring(l, 2), lua_tostring(l, -1));
            LogCallStack(l);
            return 0;
        }

        static int LuaConstNewIndex(lua_State* l) {
            lua_pushstring(l, "__name");
            if (lua_rawget(l, 1) != LUA_TSTRING)
                return 0;

            detail::LogError("const Type:[%s] member:[%s] can not been modify", lua_tostring(l, 2), lua_tostring(l, -1));
            LogCallStack(l);
            return 0;
        }

        static int LuaCast(lua_State* l) {
            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            int l_ty = lua_type(l, 1);
            if (l_ty == LUA_TLIGHTUSERDATA) {
                return 0;
            } else if (l_ty == LUA_TUSERDATA) {
                return 0;
            } else {
                return 0;
            }
        }

        static int LuaIsValid(lua_State* l) {
            int l_ty = lua_type(l, 1);
            if (l_ty == LUA_TLIGHTUSERDATA) {

            } else if (l_ty == LUA_TUSERDATA) {

            } else {
                lua_pushboolean(l, false);
            }
            return 1;
        }

        static int LuaType(lua_State* l) {
            int l_ty = lua_type(l, 1);
            if (l_ty == LUA_TLIGHTUSERDATA) {

            } else if (l_ty == LUA_TUSERDATA) {

            } else {
                lua_pushstring(l, lua_typename(l, 1));
            }
            return 1;
        }

        static int LuaGetTypeMeta(lua_State* l) {
            const auto* info = GlobalVar::GetInstance()->GetTypeInfo(lua_tostring(l, 1));
            if (info == nullptr || info->index == 0)
                return 0;

            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            lua_rawgeti(l, LUA_REGISTRYINDEX, xl->meta_table_ref_);
            lua_rawgeti(l, -1, info->index);
            lua_remove(l, -2);
            return 1;
        }

        static int LuaGetMetaList(lua_State* l) {
            xLuaState* xl = static_cast<xLuaState*>(lua_touserdata(l, lua_upvalueindex(1)));
            lua_rawgeti(l, LUA_REGISTRYINDEX, xl->meta_table_ref_);
            return 1;
        }
    };
}

xLuaGuard::xLuaGuard(xLuaState* l) : l_(l) {
    top_ = l->GetTopIndex();
}

xLuaGuard::xLuaGuard(xLuaState* l, int off) : l_(l) {
    top_ = l_->GetTopIndex() + off;
}

xLuaGuard::~xLuaGuard() {
    lua_settop(l_->GetState(), top_);
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
        detail::LightUserData ptr = detail::MakeLightPtr(lua_touserdata(state_, index));
        if (ptr.type_ == 0) {
            detail::ArrayObj* obj = detail::GlobalVar::GetInstance()->GetArrayObj(ptr.index_);
            if (obj == nullptr)
                snprintf(type_name_buf_, XLUA_MAX_TYPE_NAME_LENGTH, "nullptr");
            else
                snprintf(type_name_buf_, XLUA_MAX_TYPE_NAME_LENGTH, "%s*", obj->info_->type_name);
        }
#endif // XLUA_USE_LIGHT_USER_DATA
    } else if (l_ty == LUA_TUSERDATA) {
        detail::FullUserData* ud = static_cast<detail::FullUserData*>(lua_touserdata(state_, index));
        switch (ud->type_) {
        case detail::UserDataCategory::kValue:
            ret = ud->info_->type_name;
            break;
        case detail::UserDataCategory::kRawPtr:
        case detail::UserDataCategory::kObjPtr:
            snprintf(type_name_buf_, XLUA_MAX_TYPE_NAME_LENGTH, "%s*", ud->info_->type_name);
            break;
        case detail::UserDataCategory::kSharedPtr:
            snprintf(type_name_buf_, XLUA_MAX_TYPE_NAME_LENGTH, "std::shared_ptr<%s>", ud->info_->type_name);
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
    xLuaGuard guard(this);
    if (luaL_loadbuffer(state_, stream, strlen(stream), chunk) != LUA_OK) {
        // log err
        return false;
    }

    if (lua_pcall(state_, 0, 0, 0) != LUA_OK) {
        // log err
        return false;
    }
    return true;
}

int xLuaState::LoadGlobal(const char* path) {
    if (path == nullptr || *path == 0)
        return LUA_TNIL;

    lua_pushglobaltable(state_);
    while (const char* sub = ::strchr(path, '.')) {
        lua_pushlstring(state_, path, sub - path);
        if (lua_gettable(state_, -2) != LUA_TTABLE)
            return LUA_TNIL;

        lua_remove(state_, -2);
        path = sub + 1;
    }

    lua_pushstring(state_, path);
    int l_ty = lua_gettable(state_, -2);
    lua_remove(state_, -2);
    return l_ty;
}

bool xLuaState::SetGlobal(const char* path) {
    if (path == nullptr || *path == 0)
        return false;

    lua_pushglobaltable(state_);
    while (const char* sub = ::strchr(path, '.')) {
        lua_pushlstring(state_, path, sub - path);
        int l_ty = lua_gettable(state_, -2);
        if (l_ty == lua_isnil(state_, -1)) {
            lua_pushlstring(state_, path, sub - path);
            lua_newtable(state_);
            lua_settable(state_, -3);

            lua_pushlstring(state_, path, sub - path);
            lua_gettable(state_, -2);
            lua_remove(state_, -2);
        } else if (l_ty != LUA_TTABLE) {
            lua_pop(state_, 2);
            return false;
        }

        path = sub + 1;
    }

    lua_pushstring(state_, path);       // push key
    lua_rotate(state_, -3, 1);          // move target value to top
    lua_settable(state_, -3);           // set table field
    lua_pop(state_, 1);
    return true;
}

void xLuaState::Gc(detail::FullUserData* ud) {
    switch (ud->type_)
    {
    case detail::UserDataCategory::kValue:
        break;
    case detail::UserDataCategory::kRawPtr:
        {
            auto it = raw_ptrs_.find(detail::GetRootPtr(ud->obj_, ud->info_));
            UnRefCachce(it->second);
            raw_ptrs_.erase(it);
        }
        break;
    case detail::UserDataCategory::kSharedPtr:
        {
            auto it = shared_ptrs_.find(detail::GetRootPtr(ud->obj_, ud->info_));
            UnRefCachce(it->second);
            shared_ptrs_.erase(it);
        }
        break;
    case detail::UserDataCategory::kObjPtr:
        if (ud->info_->is_weak_obj) {
            auto& udc = weak_obj_ptrs_[ud->index_];
            if (ud == udc.user_data_)
                UnRefCachce(udc);
        } else {
            auto& udc = lua_obj_ptrs_[ud->index_];
            if (ud == udc.user_data_)
                UnRefCachce(udc);
        }
        break;
    default:
        assert(false);
        break;
    }

    ud->~FullUserData();
}

bool xLuaState::InitEnv(const std::vector<const ConstInfo*>& consts,
    const std::vector<TypeInfo*>& types,
    const std::vector<const char*>& scripts) {
    // user data ref table (weak table)
    lua_createtable(state_, XLUA_CONTAINER_INCREMENTAL, 0);
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

    // user data cache table
    lua_newtable(state_);
    lua_obj_table_ref_ = luaL_ref(state_, LUA_REGISTRYINDEX);
    assert(lua_obj_table_ref_);

    // xlua table
    lua_newtable(state_);
    PushClosure(&detail::MetaFuncs::LuaCast);
    lua_setfield(state_, -2, "Cast");
    PushClosure(&detail::MetaFuncs::LuaIsValid);
    lua_setfield(state_, -2, "IsValid");
    PushClosure(&detail::MetaFuncs::LuaType);
    lua_setfield(state_, -2, "Type");
    PushClosure(&detail::MetaFuncs::LuaGetTypeMeta);
    lua_setfield(state_, -2, "GetTypeMeta");
    PushClosure(&detail::MetaFuncs::LuaGetMetaList);
    lua_setfield(state_, -2, "GetMetaList");
    lua_setglobal(state_, "xlua");

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

    InitConsts(consts);

    for (const TypeInfo* info : types) {
        if (info->category != TypeCategory::kGlobal)
            CreateTypeMeta(info);
        if (detail::HasGlobal(info))
            CreateTypeGlobal(info);
    }

    for (const char* script : scripts) {
        DoString(script, "InitEnv");
    }

    return true;
}

void xLuaState::InitConsts(const std::vector<const ConstInfo*>& consts) {
    lua_createtable(state_, 2, 0);  // const metatable

    lua_pushcfunction(state_, &detail::MetaFuncs::LuaConstIndex);
    lua_setfield(state_, -2, "__index");
    lua_pushcfunction(state_, &detail::MetaFuncs::LuaConstNewIndex);
    lua_setfield(state_, -2, "__newindex");

    for (const ConstInfo* info : consts) {
        int num = 0;
        while (info->values[num].name)
            ++num;

        lua_createtable(state_, 0, num+1);
        SetGlobal(info->name);
        LoadGlobal(info->name);

        lua_pushstring(state_, info->name);
        lua_setfield(state_, -2, "__name");

        for (int i = 0; i < num; ++i) {
            const auto& val = info->values[i];
            switch (val.category) {
            case ConstCategory::kInteger:
                lua_pushnumber(state_, val.int_val);
                break;
            case ConstCategory::kFloat:
                lua_pushnumber(state_, val.float_val);
                break;
            case ConstCategory::kString:
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
        lua_pop(state_, 1);                 // pop table
    }

    lua_pop(state_, 1); // pop const meta table
    assert(lua_gettop(state_) == 0);
}

void xLuaState::SetTypeMember(const TypeInfo* info) {
    const TypeInfo* super = info->super;
    while (super) {
        SetTypeMember(super);
        super = super->super;
    }

    for (const TypeMember* mem = info->members; mem->category != MemberCategory::kInvalid; ++mem) {
        if (mem->category == MemberCategory::kFunction) {
            PushClosure(mem->func);
            lua_setfield(state_, -2, mem->name);
        } else if (mem->category == MemberCategory::kVariate) {
            lua_pushlightuserdata(state_, const_cast<TypeMember*>(mem));
            lua_setfield(state_, -2, mem->name);
        }
    }
}

void xLuaState::SetGlobalMember(const TypeInfo* info, bool func, bool var) {
    const TypeInfo* super = info->super;
    while (super) {
        SetGlobalMember(super, func, var);
        super = super->super;
    }

    for (const TypeMember* mem = info->globals; mem->category != MemberCategory::kInvalid; ++mem) {
        if (func && mem->category == MemberCategory::kFunction) {
            PushClosure(mem->func);
            lua_setfield(state_, -2, mem->name);
        } else if (var && mem->category == MemberCategory::kVariate) {
            lua_pushlightuserdata(state_, const_cast<TypeMember*>(mem));
            lua_setfield(state_, -2, mem->name);
        }
    }
}

void xLuaState::CreateTypeMeta(const TypeInfo* info) {
    lua_rawgeti(state_, LUA_REGISTRYINDEX, meta_table_ref_);
    lua_newtable(state_);
    lua_rawseti(state_, -2, info->index);
    lua_rawgeti(state_, -1, info->index);
    lua_remove(state_, -2);

    SetTypeMember(info);

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

void xLuaState::CreateTypeGlobal(const TypeInfo* info) {
    const char* name = info->type_name;
    lua_newtable(state_);
    bool ok = SetGlobal(info->type_name);
    assert(ok);

    int l_ty = LoadGlobal(info->type_name);
    assert(l_ty == LUA_TTABLE);

    lua_pushstring(state_, info->type_name);
    lua_setfield(state_, -2, "__name");

    SetGlobalMember(info, true, false);

    // meta table
    lua_createtable(state_, 0, 0);
    SetGlobalMember(info, false, true);

    lua_pushstring(state_, info->type_name);
    lua_setfield(state_, -2, "__name");

    PushClosure(&detail::MetaFuncs::LuaGlobalIndex);
    lua_setfield(state_, -2, "__index");

    PushClosure(&detail::MetaFuncs::LuaGlobalNewIndex);
    lua_setfield(state_, -2, "__newindex");

    lua_setmetatable(state_, -2);   // set metatable
    lua_pop(state_, 1);

    assert(lua_gettop(state_) == 0);
}

void xLuaState::PushClosure(lua_CFunction func) {
    lua_pushlightuserdata(state_, this);    // upvalue(1):xLuaSate*
    lua_pushcclosure(state_, func, 2);
}

int xLuaState::RefLuaObj(int index) {
    if (next_free_lua_obj_ == -1) {
        size_t o_s = lua_objs_.size();
        size_t n_s = o_s + XLUA_CONTAINER_INCREMENTAL;
        lua_objs_.resize(n_s, LuaObjRef{-1, 0});

        for (size_t i = o_s; i < n_s; ++i)
            lua_objs_[i].next_free_ = (int)(i + 1);
        next_free_lua_obj_ = (int)o_s;
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

XLUA_NAMESPACE_END
