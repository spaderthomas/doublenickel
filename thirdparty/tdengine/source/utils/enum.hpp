template<typename T>
bool enum_any(T value) {
	return value != T::None;
}

#ifdef __linux__
// C++ I love you, but you're bringing me down
template <typename T, bool = std::is_enum<T>::value>
struct is_flag;
template <typename T>
struct is_flag<T, true> : std::false_type { };
#define DEFINE_ENUM_FLAG_OPERATORS(EnumType) template <> struct is_flag<EnumType> : std::true_type {};

template <typename T, typename std::enable_if<is_flag<T>::value>::type* = nullptr>
T operator |(T lhs, T rhs) {
    using u_t = typename std::underlying_type<T>::type;
    return static_cast<T>(static_cast<u_t>(lhs) | static_cast<u_t>(rhs));
}
template <typename T, typename std::enable_if<is_flag<T>::value>::type* = nullptr>
T operator &(T lhs, T rhs) {
    using u_t = typename std::underlying_type<T>::type;
    return static_cast<T>(static_cast<u_t>(lhs) & static_cast<u_t>(rhs));
}
template <typename T, typename std::enable_if<is_flag<T>::value>::type* = nullptr>
bool has_flag(T lhs, T rhs) {
    using u_t = typename std::underlying_type<T>::type;
    u_t has = static_cast<u_t>(lhs) & static_cast<u_t>(rhs);
	return has != 0;
}
#endif
