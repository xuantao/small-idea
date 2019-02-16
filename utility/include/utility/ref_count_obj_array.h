#pragma once
#include "common.h"
#include <vector>

UTILITY_NAMESPACE_BEGIN

struct lua_State {};

union KGLuaArrayObj
{
    struct
    {
        int         nRefCount;  // 引用计数
        int         nLuaRef;    // lua引用编号
        lua_State*  pState;     // lua
    };

    struct
    {
        int         nNextSlot;  // 下一个空闲的编号
    };
};

static void ZeroArrayObj(KGLuaArrayObj& obj)
{
    obj.nRefCount   = 0;
    obj.nLuaRef     = 0;
    obj.pState      = nullptr;
}

class KGLuaGlobalData
{
public:
    int AllocObj(lua_State* L, int nIndex)
    {
        if (m_RefObjs.empty())
        {
            int nIndex = 0;
            m_RefObjs.resize(1024);

            for (auto& obj : m_RefObjs)
            {
                ZeroArrayObj(obj);
                obj.nNextSlot = ++nIndex;
            }
            m_RefObjs.back().nNextSlot = 0;
        }
        else if (m_RefObjs.front().nNextSlot == 0)
        {
            int nSize = (int)m_RefObjs.size();
            m_RefObjs.resize(nSize + 1024);
            m_RefObjs.front().nNextSlot = nSize;

            for (auto& obj : m_RefObjs)
            {
                ZeroArrayObj(obj);
                obj.nNextSlot = ++nSize;
            }
            m_RefObjs.back().nNextSlot = 0;
        }

        int nFreeSlot = m_RefObjs.front().nNextSlot;
        auto& refObj = m_RefObjs[nFreeSlot];
        m_RefObjs.front().nNextSlot = refObj.nNextSlot;
        refObj.nNextSlot = 0;
        refObj.nRefCount = 1;

        {
            //TODO: ref lua obj
        }

        return nFreeSlot;
    }

    bool GetObj(lua_State* L, int nSlot)
    {
        assert(nSlot > 0 && nSlot < (int)m_RefObjs.size());
        auto& rObj = m_RefObjs[nSlot];
        if (rObj.pState != L || rObj.nLuaRef == 0)
            return false;

        //TODO:
        return true;
    }

    void AddRef(int nIndex)
    {
        assert(nIndex > 0 && nIndex < (int)m_RefObjs.size());
        m_RefObjs[nIndex].nRefCount += 1;
    }

    void UnRef(int nIndex)
    {
        assert(nIndex > 0 && nIndex < (int)m_RefObjs.size());
        auto& refObj = m_RefObjs[nIndex];
        refObj.nRefCount -= 1;
        if (refObj.nRefCount == 0)
        {
            //TODO: unref
            auto& rFront = m_RefObjs.front();
            int nNextIndex = rFront.nNextSlot;
            rFront.nNextSlot = refObj.nNextSlot;
            refObj.nNextSlot = nNextIndex;
        }
    }

    void Clear(lua_State* pState)
    {

    }

private:
    std::vector<KGLuaArrayObj> m_RefObjs;
};

UTILITY_NAMESPACE_END
