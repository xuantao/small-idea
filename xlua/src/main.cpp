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
            return [](xlua::xLuaState* L, void* obj) {
                extend_member_var* self = nullptr;
                auto f = [self]() {
                    return self->a;
                };

                xlua::detail::MetaGet(L, &f, &decltype(f)::operator ());
            };
        }

        static xlua::LuaIndexer Set() {
            return nullptr;
        }
        int a;
    };
}

struct TestValue {
    virtual ~TestValue() { }
    virtual bool SharedCast(void* ptr, const xlua::TypeInfo* dst) { return false; }
    void* obj;
    xlua::TypeInfo* info;
};

template <typename Ty>
struct ValueImpl : TestValue
{
    ValueImpl(const Ty& val) : val_(val) {
        obj = &val_;
    }

    ~ValueImpl() {
    }

    Ty val_;
};

template <typename Ty>
struct ValueImpl<std::shared_ptr<Ty>> : TestValue
{
    ValueImpl(const std::shared_ptr<Ty>& val) : val_(val) {
        obj = val_.get();
    }

    ~ValueImpl() {
    }

    virtual bool SharedCast(void* ptr, const xlua::TypeInfo* dst) {
        //void* dst_ptr = info->convert_up(ptr, info, dst);
        //if (dst_ptr == nullptr)
        //    return false;

        ////*(shared_ptr<Ty>*)(ptr) =
        return false;
    }

    std::shared_ptr<Ty> val_;
};

template <typename Ty>
struct ValueImpl<Ty*> : TestValue
{
    ValueImpl(Ty* val) : val_(val) {
        obj = val_;
    }

    ~ValueImpl() {
    }

    Ty* val_;
};

template <typename Ty>
TestValue* MakeValue(const Ty& val) {
    return new ValueImpl<Ty>(val);
}

struct TestObj {
    TestObj() { printf("TestObj constructed\n"); }
    ~TestObj() { printf("TestObj destructed\n"); }
};

struct IBaseType1 {
    virtual void print1() = 0;
};

struct IBaseType2 {
    virtual void print2() = 0;
};

struct TestMulty : IBaseType1, IBaseType2 {
    void print1() override { printf("1111\n"); }
    void print2() override { printf("2222\n"); }
};

int main()
{
    //extend_member_var::Get()(nullptr,  nullptr);
    //xlua::Startup();
    //std::shared_ptr<TestObj> ptr(new TestObj());
    //TestValue* val = MakeValue(ptr);
    //
    //const TestObj* obj = ptr.get();

    //TestValue* val1 = MakeValue(ptr.get());
    ////TestValue* val2 = MakeValue(obj); // not allow const value
    //TestValue* val3 = MakeValue(TestObj());

    ////std::make_shared
    //delete val3;
    ////delete val2;
    //delete val1;
    //delete val;

    TestMulty obj;
    TestMulty* obj_ptr = &obj;
    IBaseType1* base_ptr1 = obj_ptr;
    IBaseType2* base_ptr2 = obj_ptr;

    void* void_ptr1 = base_ptr1;
    void* void_ptr2 = base_ptr2;
    TestMulty* obj_ptr1 = static_cast<TestMulty*>(base_ptr1);
    TestMulty* obj_ptr2 = static_cast<TestMulty*>(base_ptr2);

    TestMulty* obj_ptr3 = static_cast<TestMulty*>(void_ptr1);
    TestMulty* obj_ptr4 = static_cast<TestMulty*>(void_ptr2);

    printf("obj_ptr   = 0x%p\n", obj_ptr);
    printf("obj_ptr1  = 0x%p\n", obj_ptr1);
    printf("obj_ptr2  = 0x%p\n", obj_ptr2);
    printf("obj_ptr3  = 0x%p\n", obj_ptr3);
    printf("obj_ptr4  = 0x%p\n", obj_ptr4);
    printf("base_ptr1 = 0x%p\n", base_ptr1);
    printf("base_ptr2 = 0x%p\n", base_ptr2);

    system("pause");
    return 0;
}
