#pragma once
#include "xlua_config.h"
#include "xlua_def.h"
#include "xlua_state.h"

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
