#include "xlua.h"
#include "detail/global.h"

XLUA_NAMESPACE_BEGIN

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
        return;

    return global->Create();
}

xLuaState* Attach(lua_State* l) {
    auto global = detail::GlobalVar::GetInstance();
    if (global == nullptr)
        return;

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

void xLuaState::PushCacheUd(UserDataCache& cache, void* ptr, const TypeInfo* info) {
    if (!detail::IsBaseOf(info, cache.user_data_->info_)) {
        cache.user_data_->obj_ = ptr;
        cache.user_data_->info_ = info;
    }

    //TODO: push ref obj
}

void xLuaState::PushUd(UserDataCache& cache) {

}

#if XLUA_USE_LIGHT_USER_DATA
void xLuaState::_PushLightPtr(detail::LightDataPtr ptr) {

}
#else
void xLuaState::_PushWeakObjPtr(const detail::LuaUserData user_data) {

}
#endif // XLUA_USE_LIGHT_USER_DATA

XLUA_NAMESPACE_END
