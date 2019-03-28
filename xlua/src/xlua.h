#pragma once
#include "detail/traits.h"
#include "detail/state.h"
#include "xlua_def.h"
#include "xlua_obj.h"
#include "xlua_state.h"
#include <lua.hpp>
#include <string>
#include <unordered_map>

/* 扩展类型
 * void xLuaPush(xlua::xLuaState* l, const Type& val)
 * Type xLuaLoad(xlua::xLuaState* L, int index, xlua::Identity<Type>);
 * const char* xLuaTypeName(xlua::Identity<Type>);
*/

XLUA_NAMESPACE_BEGIN

bool Startup();
void Shutdown();
xLuaState* Create();
xLuaState* Attach(lua_State* l);

XLUA_NAMESPACE_END
