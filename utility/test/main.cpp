#include <stdio.h>
#include <type_traits>

/* used for extract const/normal override member functions */
struct any_type_tag;
struct mormal_type_tag;
struct const_type_tag;

struct any_type_tag
{
    friend struct mormal_type_tag;
    friend struct const_type_tag;

private:
    any_type_tag() = default;
};

struct mormal_type_tag
{
    mormal_type_tag() = default;
    mormal_type_tag(const const_type_tag&) {}

    operator any_type_tag() const { return any_type_tag(); }
};

struct const_type_tag
{
    const_type_tag() = default;
    const_type_tag(const mormal_type_tag&) {}

    operator any_type_tag() const { return any_type_tag(); }
};

static constexpr int unknown_type_tag_val = 0;       // 未知
static constexpr int normal_func_type_tag_val = 1;   // 普通函数
static constexpr int normal_var_type_tag_val = 2;    // 普通变量
static constexpr int mem_func_type_tag_val = 3;      // 成员函数
static constexpr int mem_var_type_tag_val = 4;       // 成员变量

typedef std::integral_constant<int, unknown_type_tag_val>       unknown_type_tag;
typedef std::integral_constant<int, normal_func_type_tag_val>   normal_func_type_tag;
typedef std::integral_constant<int, normal_var_type_tag_val>    normal_var_type_tag;
typedef std::integral_constant<int, mem_func_type_tag_val>      mem_func_type_tag;
typedef std::integral_constant<int, mem_var_type_tag_val>       mem_var_type_tag;

template <typename Rty, typename... Args>
struct normal_func_type : normal_func_type_tag
{
    typedef Rty return_type;
    typedef type_list<Args...> arg_list;
    typedef Rty(type)(Args...);
};

template <typename Rty>
struct normal_var_type : normal_var_type_tag
{
    typedef Rty return_type;
    return Rty type;
};

template <typename Rty, typename Cty, typename... Args>
struct mem_func_type : mem_func_type_tag
{
    typedef Rty return_type;
    typedef Cty class_type;
    typedef type_list<Args...> arg_list;
    typedef Rty(Cty::*type)(Args...);
};

template <typename Rty, typename Cty>
struct mem_var_type : mem_var_type_tag
{
    typedef Rty return_type;
    typedef Cty class_type;
    typedef Rty Cty::*type;
};

/* extractor specify the type (override functions) */
template <typename... Args>
struct func_extractor
{
    template <typename Rty, typename Cty>
    static mem_func_type<Rty, const Cty, Args...> extract(const_type_tag, Rty(Cty::*)(Args...) const);

    template <typename Rty, typename Cty>
    static mem_func_type<Rty, Cty, Args...> extract(mormal_type_tag, Rty(Cty::*)(Args...));

    template <typename Rty>
    static normal_func_type<Rty, Args...> extract(any_type_tag, Rty(*)(Args...));
};

/* not support funtion param with depend T(void) */
template <>
struct func_extractor<void>
{
    template <typename Rty, typename Cty>
    static mem_func_type<Rty, const Cty> extract(const_type_tag, Rty(Cty::*)(void) const);

    template <typename Rty, typename Cty, typename... Args>
    static mem_func_type<Rty, Cty, Args...> extract(mormal_type_tag, Rty(Cty::*)(void));

    template <typename Rty>
    static normal_func_type<Rty> extract(any_type_tag, Rty(*)(void));
};

/* for any type */
template <>
struct func_extractor<>
{
    template <typename Rty, typename Cty, typename... Args>
    static mem_func_type<Rty, const Cty, Args...> extract(const_type_tag, Rty(Cty::*const_func)(Args...) const);

    template <typename Rty, typename Cty, typename... Args>
    static mem_func_type<Rty, Cty, Args...> extract(mormal_type_tag, Rty(Cty::*func)(Args...));

    template <typename Rty, typename... Args>
    static normal_func_type<Rty, Args...> extract(any_type_tag, Rty(*func)(Args...));
};

struct var_extractor
{
    template <typename Rty>
    static auto extract(any_type_tag, Rty*) ->
        typename std::enable_if<!std::is_function<Rty*>::value, normal_var_type<Rty>>::type;

    template <typename Rty, typename Cty>
    static auto extract(any_type_tag, Rty Cty::*) ->
        typename std::enable_if<!std::is_member_function_pointer<Rty Cty::*>, mem_var_type<Rty, Cty>>::type;
};

template <typename... Args>
struct extractor : var_extractor, func_extractor<Args...>
{
    static unknown_type_tag extract(any_type_tag, std::nullptr_t);
};

struct lua_State;
struct luna_mem_func_tag {};
struct luna_normal_func_tag {};

template <typename Rty, typename Cty>
struct luna_mem_func_info : luna_mem_func_tag
{
    static_assert(std::is_same<Rty, int>::value || std::is_same<Rty, void>::value,
        "lua member function's return type must be int/void");

    typedef Rty return_type;
    typedef Cty class_type;
};

template <typename Rty>
struct luna_normal_func_info : luna_mem_func_tag
{
    static_assert(std::is_same<Rty, int>::value || std::is_same<Rty, void>::value,
        "lua function's return type must be int/void");
    typedef Rty return_type;
};

template <typename... Args>
struct luna_extractor : extractor<Args...>
{
    template <typename Rty, typename Cty>
    static normal_func_type<Rty> extract(mormal_type_tag, Rty(Cty::*func)(lua_State*));

    template <typename Rty>
    static normal_func_type<Rty> extract(mormal_type_tag, Rty(*func)(lua_State*));
};

struct wrapper_call
{
    template <typename func_info>
    static auto call() ->
        typename std::enable_if<
            std::is_base_of<mem_func_type_tag, func_info>::value && std::is_same<void, typename func_info::return_type>::value,
            int
        >::type
    {
        printf("call member function\n");
        return 1;
    }

    template <typename func_info>
    static auto call() -> typename std::enable_if<std::is_base_of<normal_func_type_tag, func_info>::value && std::is_same<void, typename func_info::return_type>::value, int>::type
    {
        printf("call global_function\n");
        return 1;
    }
};


struct test_obj_member_base
{
    void doWork()
    {
        printf("222222222\n");
    }
};

struct test_obj_member : public test_obj_member_base
{
    void print()
    {
        printf("11111111\n");
    }

    void test_const() const { printf("3333333\n"); }
    void test_const() { printf("4444444\n"); }
    void test_normal() { printf("5555555\n"); }
    void test_normal(int) { printf("5555555\n"); }

    int a;
    const int b = 0;
};

int global_c = 0;
const int global_d = 0;

void test_const() { printf("5555555\n"); }

template <typename... Args>
struct type_list { };

int main(int argc, char* argv[])
{
    //typedef decltype(func_extractor<>::extract(const_type_tag(), &test_const)) mem_func_1;
    //constexpr bool is_const_1 = std::is_const<typename mem_func_1::return_type>::value;
    //printf("is_const1 : %s\n", is_const_1 ? "true" : "false");

    //typedef decltype(func_extractor<>::extract(const_type_tag(), &test_obj_member::test_const)) mem_func_2;
    //constexpr bool is_const_2 = std::is_const<typename mem_func_2::class_type>::value;
    //printf("is_const2 : %s\n", is_const_2 ? "true" : "false");

    //typedef decltype(func_extractor<>::extract(mormal_type_tag(), &test_obj_member::test_const)) mem_func_3;
    //constexpr bool is_const_3 = std::is_const<typename mem_func_3::class_type>::value;
    //printf("is_const3 : %s\n", is_const_3 ? "true" : "false");

    //typedef decltype(func_extractor<void>::extract(mormal_type_tag(), &test_obj_member::test_normal)) mem_func_4;
    //constexpr bool is_const_4 = std::is_const<typename mem_func_4::class_type>::value;
    //printf("is_const4 : %s\n", is_const_4 ? "true" : "false");

    //printf("member data\n");
    //typedef decltype(func_extractor<>::extract(mormal_type_tag(), &test_obj_member::a)) mem_func_5;
    //typedef decltype(func_extractor<>::extract(mormal_type_tag(), &test_obj_member::b)) mem_func_6;
    //printf("global\n");
    //typedef decltype(func_extractor<>::extract(mormal_type_tag(), &global_c)) mem_func_7;
    //typedef decltype(func_extractor<>::extract(mormal_type_tag(), &global_d)) mem_func_8;

    //auto mem = (void (test_obj_member::*)(int))&test_obj_member::test_normal;

    //printf("member data\n");
    //typedef decltype(func_extractor<void>::extract(mormal_type_tag(), &test_obj_member::a)) mem_func_5;
    //typedef decltype(func_extractor<void>::extract(mormal_type_tag(), &test_obj_member::b)) mem_func_6;
    //printf("global\n");
    //typedef decltype(func_extractor<void>::extract(mormal_type_tag(), &global_c)) mem_func_7;
    //typedef decltype(func_extractor<void>::extract(mormal_type_tag(), &global_d)) mem_func_8;

    //wrapper_call::call<mem_func_1>();
    //wrapper_call::call<mem_func_2>();
    return 0;
}
