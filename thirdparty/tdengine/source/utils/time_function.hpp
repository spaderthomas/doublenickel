namespace timer_detail {
	template <typename F, typename... Args>
	struct result_of {
		using type = typename std::invoke_result<F, Args...>::type;
	};

	template <typename F, typename... Args>
	using result_of_t = typename result_of<F, Args...>::type;

	template <typename T>
	struct is_void : std::is_void<T> {};

	template <typename F, typename... Args>
	struct returns_void : is_void<result_of_t<F, Args...>> {};
}

using namespace timer_detail;

template <typename F, typename... Args>
auto time_function(F&& f, Args&&... args)
-> typename std::enable_if<!returns_void<F, Args...>::value, std::tuple<typename result_of<F, Args...>::type, std::chrono::milliseconds>>::type
{
    auto start = std::chrono::high_resolution_clock::now();
    auto result = f(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    return std::make_tuple(result, duration);
}

template <typename F, typename... Args>
auto time_function(F&& f, Args&&... args)
-> typename std::enable_if<returns_void<F, Args...>::value, std::chrono::milliseconds>::type
{
    auto start = std::chrono::high_resolution_clock::now();
    f(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	return duration;
}

