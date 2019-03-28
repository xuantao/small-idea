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
        return nullptr;

    return global->Create();
}

xLuaState* Attach(lua_State* l) {
    auto global = detail::GlobalVar::GetInstance();
    if (global == nullptr)
        return nullptr;

    return global->Attach(l);
}

XLUA_NAMESPACE_END
