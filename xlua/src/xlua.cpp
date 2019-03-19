#include "xlua.h"
#include "detail/global.h"

XLUA_NAMESPACE_BEGIN

bool Startup() {
    return detail::GlobalVar::Startup();
}

void Shutdown() {
    auto global = detail::GlobalVar::GetInstance();
    if (global)
        global->Purge();
}

xLuaState::xLuaState(lua_State* l, bool attach) {

}

xLuaState::~xLuaState() {

}

bool xLuaState::_TryPushSharedPtr(void* root) const {
    auto it = shared_ptrs_.find(root);
    if (it == shared_ptrs_.cend())
        return false;
    return true;
}

void xLuaState::_PushSharedPtr(void* root, detail::LuaUserData* data) {
    shared_ptrs_.insert(std::make_pair(root, 1));
}

void xLuaState::_PushUniquePtr(detail::LuaUserData* user_data) {
    printf("push unique_ptr\n");
}

void xLuaState::_PushValue(detail::LuaUserData* user_data) {

}

bool xLuaState::_TryPushRawPtr(void* root) const {
    auto it = raw_ptrs_.find(root);
    if (it == raw_ptrs_.cend())
        return false;
    return true;
}

void xLuaState::_PushRawPtr(void* root, detail::LuaUserData* user_data) {
    raw_ptrs_.insert(std::make_pair(root, 1));
}

#if XLUA_USE_LIGHT_USER_DATA
void xLuaState::_PushLightPtr(detail::LightDataPtr ptr) {

}
#else
void xLuaState::_PushWeakObjPtr(const detail::LuaUserData user_data) {

}
#endif // XLUA_USE_LIGHT_USER_DATA

XLUA_NAMESPACE_END
