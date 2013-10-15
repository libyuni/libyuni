
# C++0x
check_cxx_compiler_flag("-std=c++0x" YUNI_HAS_GCC_CPP0X_SUPPORT)
if(YUNI_HAS_GCC_CPP0X_SUPPORT)
	if(MINGW)
		set(YUNI_GCC_CPP0X_FLAG "-std=gnu++0x")
	else()
		set(YUNI_GCC_CPP0X_FLAG "-std=c++0x")
	endif()
	LIBYUNI_CONFIG_CFLAG("both" "core"	"${YUNI_GCC_CPP0X_FLAG}")
endif()



# C++11 nullptr
if(YUNI_HAS_GCC_CPP0X_SUPPORT)
	if(MINGW)
		set(CMAKE_REQUIRED_FLAGS "-std=gnu++0x")
	else()
		set(CMAKE_REQUIRED_FLAGS "-std=c++0x")
	endif()
endif()


check_cxx_source_compiles("
	#include <cstddef>
	typedef decltype(nullptr) NullPtr;
	class A {}; int main() {A* a = nullptr;return 0;}" YUNI_HAS_NULLPTR)


check_cxx_source_compiles("
	#include <functional>

	class Dummy {};

	namespace std
	{
		template<>
		struct hash<Dummy>
		{
			size_t operator() (const Dummy& ) const
			{
				return 0;
			}
		};

	} // namespace std

	int main() {return 0;}" YUNI_HAS_STL_HASH)


check_cxx_source_compiles("
	#include <unordered_map>

	int main() {std::unordered_map<int, int> map;return 0;}" YUNI_HAS_STL_UNORDERED_MAP)

check_cxx_source_compiles("
	#include <unordered_set>

	int main() {std::unordered_set<int> map;return 0;}" YUNI_HAS_STL_UNORDERED_SET)



if (MSVC)
	# MSVC __forceinline
	check_cxx_source_compiles("
		__forceinline static int max(int x, int y) { return x > y ? x : y;}
		int main(void)
		{
			return max(42, 0);
		} "  YUNI_HAS_MSVC_FORCE_INLINE)
else()
	# gcc __attribute__((always_inline))
	check_cxx_source_compiles("
		__attribute__((always_inline)) static int max(int x, int y) { return x > y ? x : y;}
		int main(void)
		{
			return max(42, 0);
		} "  YUNI_HAS_GCC_ALWAYS_INLINE)
endif()



check_cxx_source_compiles("
	class IBase
	{
	public:
		virtual void foo() {}
	};
	class Foo : public IBase
	{
	public:
		virtual void foo() override {}
	};

	int main() {Foo foo;return 0;}" YUNI_HAS_CPP_KEYWORD_OVERRIDE)


check_cxx_source_compiles("
	class IBase
	{
	public:
		virtual void foo() {}
	};
	class Foo : public IBase
	{
	public:
		virtual void foo() final {}
	};

	int main() {Foo foo;return 0;}" YUNI_HAS_CPP_KEYWORD_FINAL)


check_cxx_source_compiles("
	class Foo
	{
	public:
		Foo(Foo&& other)
		{}
	};

	int main() {Foo foo;return 0;}" YUNI_HAS_CPP_MOVE_CONSTRUCTOR)


check_cxx_source_compiles("
	int main()
	{
		static_assert(true, \"test\");
		return 0;
	}" YUNI_HAS_CPP_STATIC_ASSERT)


check_cxx_source_compiles("
	#include <atomic>
	int main() {
		std::atomic_int_fast32_t afint32;
		std::atomic_int_fast64_t afint64;
		return 0;
	}" YUNI_HAS_STD_ATOMIC)


# constexpr
#if(YUNI_HAS_GCC_CPP0X_SUPPORT)
#	set(CMAKE_REQUIRED_FLAGS_SAVE ${CMAKE_REQUIRED_FLAGS})
#	set(CMAKE_REQUIRED_FLAGS "${YUNI_GCC_CPP0X_FLAG}")
#	check_cxx_source_compiles("
#		class Foo
#		{
#		public:
#			constexpr static int dummy = 42;
#		};
#		int main() {return 0;}"
#		YUNI_HAS_CONSTEXPR)
#	set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_SAVE})
#else()
#	check_cxx_source_compiles("
#	class Foo
#		{
#		public:
#			constexpr static int dummy = 42;
#		};
#		int main() {return 0;}"
#		YUNI_HAS_CONSTEXPR)
#endif()


