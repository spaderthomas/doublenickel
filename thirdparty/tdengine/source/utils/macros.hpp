#define DN_MAX(a, b) (a) > (b) ? (a) : (b)
#define DN_MIN(a, b) (a) > (b) ? (b) : (a)

#define DN_ARR_LEN(carray) (sizeof((carray)) / sizeof((carray)[0]))

#define dn_macro_str(x) #x
#define _dn_macro_cat(x, y) x##y
#define dn_macro_cat(x, y) _dn_macro_cat(x, y)
#define dn_unique_name() dn_macro_cat(__dn_unique_name__, __LINE__)

#define dn_for(it, n) for (u32 it = 0; it < (n); it++)
#define dn_for_arr(arr, it) for (u32 it = 0; it < DN_ARR_LEN(arr); it++)
#define dn_for_each_n(arr, it, n) for (auto it = (arr); it < (arr) + n; it++)
#define dn_for_each(arr, it) dn_for_each_n(arr, it, DN_ARR_LEN(arr))
#define dn_for_anon(arr) for (u32 dn_unique_name() = 0; dn_unique_name() < DN_ARR_LEN(arr); dn_unique_name()++) 

#define dn_typedef_fn(return_type, name, ...) typedef return_type(*name)(__VA_ARGS__)

#define DN_API extern "C" __declspec(dllexport)
#define DN_IMP

#define DN_ALIGN(n) __declspec(align(n))
#define DN_ASSERT(condition) assert(condition)
#define DN_UNTESTED() DN_ASSERT(false)

#define dn_zero_initialize() { 0 }

#define dn_type_name(t) (#t)

#define dn_enum_flags(t) DEFINE_ENUM_FLAG_OPERATORS(t)

// Defer works by using (abusing...?) RAII; define a struct which holds a function and calls
// that function in its destructor. Then, use __LINE__ to make an instance of that struct with
// a unique name and use operator overloading with a lambda to get nice syntax.
template <typename F>
struct Defer {
    Defer( F f ) : f( f ) {}
    ~Defer( ) { f( ); }
    F f;
};

template <typename F>
Defer<F> makeDefer( F f ) {
    return Defer<F>( f );
};

#define _dn_defer( line ) dn_macro_cat(dn_defer_, line)

struct dn_defer_dummy { };
template<typename F>
Defer<F> operator+( dn_defer_dummy, F&& f )
{
    return makeDefer<F>( std::forward<F>( f ) );
}

#define dn_defer auto _dn_defer( __LINE__ ) = dn_defer_dummy( ) + [ & ]( )


#define dn_quad_literal(top, bottom, left, right) \
    {                                     \
        { left, top },                    \
        { left, bottom },                 \
        { right, bottom },                \
                                          \
        { left, top },                    \
        { right, bottom },                \
        { right, top }                    \
    }                                     
