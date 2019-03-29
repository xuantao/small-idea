#pragma once
#include <stdio.h>
#include <cassert>

/* 64位系统开启LIGGHT_USER_DATA
 * 导出对象指针使用LightUserData代替FullUserData
*/
#ifndef XLUA_USE_LIGHT_USER_DATA
    #define XLUA_USE_LIGHT_USER_DATA 1
#endif

/* 检查系统是否支持 */
#if XLUA_USE_LIGHT_USER_DATA
    #if INTPTR_MAX != INT64_MAX
        #define XLUA_USE_LIGHT_USER_DATA 0
    #endif
#endif

/* 配置是否支持弱指针
 * 修改基类定义宏并实例化基础接口
*/
#ifndef XLUA_ENABLE_WEAKOBJ
    #define XLUA_ENABLE_WEAKOBJ 0
#endif

/* 支持弱队像必要的一些实现 */
#if !XLUA_ENABLE_WEAKOBJ
    #define XLUA_WEAK_OBJ_BASE_TYPE void
    template <typename Ty> struct xLuaWeakObjPtr {
        xLuaWeakObjPtr(Ty*) {}
        Ty* Get() const { return nullptr; }
    };

    inline int xLuaAllocWeakObjIndex(void* val) { assert(false); return -1; }
    inline int xLuaGetWeakObjSerialNum(int index) { assert(false); return 0; }
    inline void* xLuaGetWeakObjPtr(int index) { assert(false); return nullptr; }
#else // XLUA_WEAK_OBJ_BASE_TYPE
    #define XLUA_WEAK_OBJ_BASE_TYPE
    template <typename Ty> using xLuaWeakObjPtr =;
    int xLuaAllocWeakObjIndex(XLUA_WEAK_OBJ_BASE_TYPE* val);
    int xLuaGetWeakObjSerialNum(int index);
    XLUA_WEAK_OBJ_BASE_TYPE* xLuaGetWeakObjPtr(int index);
#endif // XLUA_ENABLE_WEAKOBJ

/* 配置日志输出 */
inline void xLuaLogError(const char* err) {
    printf("xlua_error:%s", err);
}

/* 名字空间配置 */
#define XLUA_NAMESPACE_BEGIN    namespace xlua {
#define XLUA_NAMESPACE_END      } // namespace xlua
#define XLUA_USE_NAMESPCE       xlua::

/* 容器容量增量 */
#define XLUA_CONTAINER_INCREMENTAL  1024
/* 类型名称最大字节数 */
#define XLUA_MAX_TYPE_NAME_LENGTH   256
/* 输出日志最大缓存字节数 */
#define XLUA_MAX_BUFFER_CACHE       1024
