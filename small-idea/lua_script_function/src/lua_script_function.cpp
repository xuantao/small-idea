#include <stdio.h>
#include <assert.h>
#include <vector>
#include <functional>
#include <type_traits>
#include <typeinfo>

class KScriptCenter;
struct ILuaScriptEx;
struct KGScriptFuncData;

uint32_t g_StringHash(const char*) { return 0; }

struct KGScriptFileData
{
    KGScriptFileData(uint32_t scriptID, const char* pName)
        : nScriptID(scriptID), pFilename(pName)
    { }

    KGScriptFileData(const KGScriptFileData&) = delete;
    KGScriptFileData& operator = (const KGScriptFileData&) = delete;

    const uint32_t      nScriptID;
    const char* const   pFilename;
    std::vector<KGScriptFuncData*> Funcs;
};

struct KGScriptFuncData
{
    KGScriptFuncData(KGScriptFileData* pScriptFile, const char* pName)
        : pFuncName(pName)
        , nFuncID(g_StringHash(pName))
        , pLua(nullptr)
        , nRefID(0)
    {
        pScriptFile->Funcs.push_back(this);
    }

    KGScriptFuncData(const KGScriptFuncData&) = delete;
    KGScriptFuncData& operator = (const KGScriptFuncData&) = delete;

    const char* const   pFuncName;
    const uint32_t      nFuncID;
    ILuaScriptEx*       pLua;
    int                 nRefID;
};

template <typename Rty, typename... Args>
class KGScriptFunc : private KGScriptFuncData
{
public:
    typedef std::pair<bool, Rty> RetType;

    KGScriptFunc(KGScriptFileData* pScriptFile, const char* pFuncName)
        : KGScriptFuncData(pScriptFile, pFuncName)
    { }

    inline explicit operator bool() const { return nRefID != 0; }

    RetType operator () (Args&&... args) const
    {
        printf("1111111111111111\n");
        RetType ret;
        return ret;
    }
};

template <typename... Args>
class KGScriptFunc<void, Args...> : private KGScriptFuncData
{
public:
    KGScriptFunc(KGScriptFileData* pScriptFile, const char* pFuncName)
        : KGScriptFuncData(pScriptFile, pFuncName)
    { }

    inline explicit operator bool() const { return nRefID != 0; }

    bool operator () (Args&&... args) const
    {
        printf("2222222222222222\n");
        return true;
    }
};

struct KGStaticScriptFile : KGScriptFileData
{
    KGStaticScriptFile(const char* pFilename)
        : KGScriptFileData(g_StringHash(pFilename), pFilename)
    { }
};

struct KGScriptFile : KGScriptFileData
{
    KGScriptFile(size_t nHash, KScriptCenter* pScriptCenter, uint32_t nScriptID, const char* pFilename)
        : KGScriptFileData(nScriptID, pFilename)
        , nTypeHash(nHash)
        , pCenter(pScriptCenter)
    { }

    int AddRef() { return ++ nRefCount; }

    void Release()
    {
        --nRefCount;
        if (nRefCount == 0)
            delete this;
        (void)(pCenter);
    }
    const size_t nTypeHash;
    int nRefCount;
    KScriptCenter* pCenter;
};

template <typename Ty>
struct KGScriptFilePtr
{
    static_assert(std::is_base_of<KGScriptFile, Ty>::value, "type must be inherited from KGScriptFile");

    KGScriptFilePtr() : pScript(Dummy()) { }

    KGScriptFilePtr(Ty* pVal) : pScript(pVal)
    {
        if (pScript == nullptr) pScript = Dummy();
    }

    KGScriptFilePtr(const KGScriptFilePtr& Other) : pScript(Other.pScript)
    {
        pScript->AddRef();
    }

    KGScriptFilePtr& operator = (const KGScriptFilePtr& Other)
    {
        if (*this) pScript->Release();

        pScript = Other.pScript;
        pScript->AddRef();
    }

    ~KGScriptFilePtr()
    {
        if (*this) pScript->Release();
    }

    inline const Ty* Get() const { return pScript; }
    inline const Ty* operator -> () const { return pScript; }
    inline const Ty& operator * () const { return *pScript; }
    inline explicit operator bool() const { return pScript != Dummy(); }

private:
    static inline Ty* Dummy()
    {
        static Ty s_Dummy(nullptr, 0, nullptr);
        return &s_Dummy;
    }

    Ty* pScript;
};

template <typename Ty> inline bool operator == (const KGScriptFilePtr<Ty>& l, const KGScriptFilePtr<Ty>& r) { return l.Get() == r.Get(); }
template <typename Ty> inline bool operator != (const KGScriptFilePtr<Ty>& l, const KGScriptFilePtr<Ty>& r) { return l.Get() != r.Get(); }
template <typename Ty> inline bool operator == (const KGScriptFilePtr<Ty>& ptr, std::nullptr_t) { return !(bool)ptr; }
template <typename Ty> inline bool operator != (const KGScriptFilePtr<Ty>& ptr, std::nullptr_t) { return (bool)ptr; }
template <typename Ty> inline bool operator == (std::nullptr_t, const KGScriptFilePtr<Ty>& ptr) { return !(bool)ptr; }
template <typename Ty> inline bool operator != (std::nullptr_t, const KGScriptFilePtr<Ty>& ptr) { return (bool)ptr; }
template <typename Ty> inline bool operator == (const KGScriptFilePtr<Ty>& ptr, const Ty* pObj) { return ptr.Get() == pObj; }
template <typename Ty> inline bool operator != (const KGScriptFilePtr<Ty>& ptr, const Ty* pObj) { return ptr.Get() != pObj; }
template <typename Ty> inline bool operator == (const Ty* pObj, const KGScriptFilePtr<Ty>& ptr) { return pObj == ptr.Get(); }
template <typename Ty> inline bool operator != (const Ty* pObj, const KGScriptFilePtr<Ty>& ptr) { return pObj != ptr.Get(); }

class KScriptCenter
{
public:
    template <typename Ty>
    Ty& StaticScript()
    {
        static_assert(std::is_base_of<KGScriptFileData, Ty>::value, "");
        static int index = ++s_indexGen;

        KGScriptFileData* pScript = nullptr;

        if (vecTables.size() <= index || vecTables[index] == nullptr)
        {
            pScript = new Ty();
            if (vecTables.size() < index)
                vecTables.resize(index * 2, nullptr);
            vecTables[index] = pScript;
        }
        else
        {
            pScript = vecTables[index];
        }

        return *((Ty*)pScript);
    }

    template <typename Ty>
    inline KGScriptFilePtr<Ty> Create(const char* lpFilename) { return Create<Ty>(g_StringHash(lpFilename), lpFilename); }

    template <typename Ty>
    inline KGScriptFilePtr<Ty> Create(uint32_t nScriptID) { return Create<Ty>(nScriptID, ""); }

private:
    template <typename Ty>
    KGScriptFilePtr<Ty> Create(uint32_t nScriptID, const char* lpFilename)
    {
        static_assert(std::is_base_of<KGScriptFileData, Ty>::value, "type must be inherited from KGScriptFile");
        Ty* pScript = new Ty(this, nScriptID, lpFilename);
        return pScript;
    }

    void Destory(KGScriptFileData* pScript)
    {
        delete pScript;
    }

    static int s_indexGen;
    std::vector<KGScriptFileData*> vecTables;
};
int KScriptCenter::s_indexGen = 0;

#define LUA_STATIC_SCRIPT_FILE_BEGIN(ClassName, ScriptFile)         \
    class ClassName : private KGStaticScriptFile                    \
    {                                                               \
    private:                                                        \
        ClassName() : KGStaticScriptFile(ScriptFile) { }            \
    public:                                                         \
        friend class KScriptCenter

#define LUA_SCRIPT_FILE_BEGIN(ClassName)                            \
    class ClassName : private KGScriptFile                          \
    {                                                               \
    private:                                                        \
        ClassName(KScriptCenter* pCenter, uint32_t nScriptID, const char* pFilename)        \
            : KGScriptFile(typeid(ClassName).hash_code(), pCenter, nScriptID, pFilename)    \
        { }                                                         \
    public:                                                         \
        friend class KScriptCenter;                                 \
        friend struct KGScriptFilePtr<ClassName>

#define LUA_SCRIPT_FILE_FUNC(Rty, Name, ...)                        \
    KGScriptFunc<Rty, __VA_ARGS__> Name = { this, #Name }

#define LUA_SCRIPT_FILE_END()                                       \
    }

LUA_STATIC_SCRIPT_FILE_BEGIN(XxxScript, "xxxx");
LUA_SCRIPT_FILE_FUNC(int, OnNewPlayerInitial, int);
LUA_SCRIPT_FILE_FUNC(void, CallB, int, int);
LUA_SCRIPT_FILE_END();

LUA_SCRIPT_FILE_BEGIN(YyyScript);
LUA_SCRIPT_FILE_FUNC(int, CallA, int);
LUA_SCRIPT_FILE_FUNC(void, CallB, int, int);
LUA_SCRIPT_FILE_END();

LUA_SCRIPT_FILE_BEGIN(ZzzScript);
LUA_SCRIPT_FILE_FUNC(int, CallA, int);
LUA_SCRIPT_FILE_FUNC(void, CallB, int, int);
LUA_SCRIPT_FILE_END();

int main(int argc, char* argv[])
{
    KScriptCenter mgr;
    auto pair = mgr.StaticScript<XxxScript>().OnNewPlayerInitial(1);
    if (pair.first)
        printf("%d\n", pair.second);

    mgr.StaticScript<XxxScript>().CallB(1, 2);

    auto ptr = mgr.Create<YyyScript>(1);
    ptr->CallA(1);
    ptr->CallB(1, 2);

    auto ptr2 = mgr.Create<ZzzScript>(1);
    ptr2->CallA(1);
    ptr2->CallB(1, 2);

    system("pause");
    return 1;
}
