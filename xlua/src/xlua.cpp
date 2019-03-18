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

XLUA_NAMESPACE_END
