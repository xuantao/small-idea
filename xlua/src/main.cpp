#include <stdio.h>
#include "xlua.h"
#include "xlua_export.h"

struct TestType1
{
    int ia;
};

namespace {
    struct extend_member_var {
        static xlua::LuaIndexer Get() {
            return [](xlua::xLuaState* L, void* obj, const xlua::TypeInfo*) {
                extend_member_var* self = nullptr;
                auto f = [self]() {
                    return self->a;
                };

                //xlua::detail::MetaGet(L, &f, &decltype(f)::operator ());
            };
        }

        static xlua::LuaIndexer Set() {
            return nullptr;
        }
        int a;
    };
}

enum TestEnum
{
    kValue1,
    kValue2,
};

enum class TestEnum2
{
    kValue11,
    kValue12,
};

struct TestNone
{};

struct TestLuaExport
{
    int ia;
    void test_call() {}

    void test_enum(TestEnum2) {}
    void test_enum(int) {}
    TestNone GetNone() { return TestNone(); }

    static void Print() { }

    int TestLua(xlua::xLuaState* l) { return 0; }
    int TestLua(xlua::xLuaState* l) const { return 0; }
};

bool TestExtend(TestLuaExport* o) { return false; }
int TestExtend2(TestLuaExport* o, xlua::xLuaState* l) { return 0; }

XLUA_EXPORT_ENUM_BEGIN(TestEnum2)
XLUA_EXPORT_ENUM_VAR(kValue11)
XLUA_EXPORT_ENUM_VAR_AS(ttt, kValue12)
XLUA_EXPORT_ENUM_END()

XLUA_DECLARE_EXTERNAL_CLASS(TestLuaExport);

XLUA_EXPORT_EXTERNAL_CLASS_BEGIN(TestLuaExport)
XLUA_EXPORT_MEMBER_FUNC(test_call)
XLUA_EXPORT_MEMBER_FUNC_AS(test_enum_e, test_enum, TestEnum2)
XLUA_EXPORT_MEMBER_FUNC_AS(test_enum_i, test_enum, int)
//XLUA_EXPORT_MEMBER_FUNC(GetNone)
XLUA_EXPORT_MEMBER_FUNC(Print)
XLUA_EXPORT_MEMBER_FUNC(TestLua)
XLUA_EXPORT_MEMBER_FUNC_EXTEND(TestExtend, TestExtend)
XLUA_EXPORT_MEMBER_FUNC_EXTEND(TestExtend2, TestExtend2)
XLUA_EXPORT_MEMBER_VAR(ia)
XLUA_EXPORT_EXTERNAL_CLASS_END()

struct LightDataPtr {
    union {
        struct {
            void* ptr_;
        };
        struct {
            struct {
                uint32_t serial_;
            };
            struct {
                uint32_t index_ : 24;
                uint32_t type_ : 8;
            };
        };
    };
};

namespace internal {
    template <typename Ty>
    struct FriendTest;
}

struct TestFriend {
    template <typename Ty>
    friend struct internal::FriendTest;

    template <typename Ty>
    void Do() {
        internal::FriendTest<Ty>::Do(this);
    }

private:
    void call() { printf("friend call\n"); }
};

namespace internal {
    template <typename Ty>
    struct FriendTest
    {
        static void Do(TestFriend* o) {
            o->call();
        }
    };
}

int main()
{
    constexpr size_t s = sizeof(LightDataPtr);
    xlua::Startup();
    xlua::xLuaState* l = xlua::Create();

    TestLuaExport* ptr = nullptr;
    TestLuaExport obj;

    std::shared_ptr<TestLuaExport> ptr1;
    std::unique_ptr<TestLuaExport> ptr2;

    l->Push(&obj);
    l->Push(obj);
    l->Push(ptr1);
    //l->Push(ptr2);

    ptr = l->Load<TestLuaExport*>(1);
    obj = l->Load<TestLuaExport>(1);
    ptr1 = l->Load<std::shared_ptr<TestLuaExport>>(1);
    //ptr1 = l->Load<std::unique_ptr<TestLuaExport>>(1);

    constexpr char ar[2] ={0, 1};
    static_assert(ar[1] == 1, "222");

    TestFriend tf;
    tf.Do<int>();

    xlua::xLuaTable tab;
    xlua::xLuaFunction func;
    l->Call(std::tie(), 1, 1.0f, false, "xxx");
    l->Call("Global.lua.xxx", std::tie(), 1, 1.0f, true);
    l->Call(func, std::tie(), 1, 1.0f, true);
    l->Call(tab, "xxxx", std::tie(), 1, 1.0f, true);

    //struct TestInner
    //{
    //    static void Call() {

    //    }

    //    template <typename Ty>
    //    int DoCall(Ty f) {
    //        return 0;
    //    }
    //};

    system("pause");
    return 0;
}
