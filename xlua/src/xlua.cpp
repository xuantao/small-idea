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

        if (dst != buf && len > 0)
            dst[-1] = 0;    // remove last '\n'
    };

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
        }
        else {
            ret.info = GlobalVar::GetInstance()->GetExternalTypeInfo(ptr.type_);
            if (ret.info->is_weak_obj) {
                if (ptr.serial_ != xLuaGetWeakObjSerialNum(ptr.index_)) {
                    // log err
                    return false;
                }

                ret.obj = xLuaGetWeakObjPtr(ptr.index_);
            }
            else {
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

            xLuaState* xl = _XLUA_GET_STATE(l);
            if (xl == nullptr) {
                // log err
                return 0;
            }

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

            xLuaState* xl = _XLUA_GET_STATE(l);
            if (xl == nullptr) {
                // log err
                return 0;
            }

            LuaUserData* ud = (LuaUserData*)lua_touserdata(l, 1);
            mem->setter(xl, ud->obj_);
            return 0;
        }

        static int LuaGc(lua_State* l) {
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

            xLuaState* xl = _XLUA_GET_STATE(l);
            if (xl == nullptr) {
                // log err
                return 0;
            }

            lua_rawgeti(l, LUA_REGISTRYINDEX, xl->meta_table_index_);
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

            xLuaState* xl = _XLUA_GET_STATE(l);
            if (xl == nullptr) {
                //TODO: log error
                return 0;
            }

            lua_rawgeti(l, LUA_REGISTRYINDEX, xl->meta_table_index_);
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

xLuaState::xLuaState(lua_State* l, bool attach) {

}

xLuaState::~xLuaState() {

}

void xLuaState::Release() {
    auto global = detail::GlobalVar::GetInstance();
    assert(global == nullptr);
    global->Destory(this);
}

int xLuaState::GetTopIndex() const
{
    return 0;
}

void xLuaState::LogCallStack() const {

}

void xLuaState::GetCallStack(char* buf, size_t size) const {

}

const char* xLuaState::GetTypeName(int index) const {
    return "";
}

LuaValueType xLuaState::LoadGlobal(const char* path) {
    return LuaValueType::kNill;
}

void xLuaState::SetGlobal(const char* path) {

}

LuaValueType xLuaState::LoadTableField(int index, int field) {
    return LuaValueType::kNill;
}

LuaValueType xLuaState::LoadTableField(int index, const char* field) {
    return LuaValueType::kNill;
}

void xLuaState::SetTableField(int index, int field) {

}

void xLuaState::SetTableField(int index, const char* field) {

}

bool xLuaState::_TryPushSharedPtr(void* root, void* ptr, const TypeInfo* info) {
    auto it = shared_ptrs_.find(root);
    if (it == shared_ptrs_.cend())
        return false;

    PushCacheUd(it->second, ptr, info);
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

    PushCacheUd(it->second, ptr, info);
    return true;
}

void xLuaState::_PushRawPtr(void* root, detail::LuaUserData* user_data) {
    auto& cache = raw_ptrs_[root];
    cache.user_data_ = user_data;
    PushUd(cache);

}

void* xLuaState::AllocUserData(size_t size) {
    return new int8_t[size];
}

void xLuaState::PushCacheUd(UserDataCache& cache, void* ptr, const TypeInfo* info) {
    if (!detail::IsBaseOf(info, cache.user_data_->info_)) {
        cache.user_data_->obj_ = ptr;
        cache.user_data_->info_ = info;
    }

    //TODO: push ref obj
}

void xLuaState::PushUd(UserDataCache& cache) {

}

TypeMember* xLuaState::GetMetaMember() {
    return nullptr;
}

void xLuaState::Gc(detail::LuaUserData* user_data) {

}

#if XLUA_USE_LIGHT_USER_DATA
void xLuaState::_PushLightPtr(detail::LightDataPtr ptr) {

}
#else
void xLuaState::_PushWeakObjPtr(const detail::LuaUserData user_data) {

}
#endif // XLUA_USE_LIGHT_USER_DATA

XLUA_NAMESPACE_END
