#pragma once

#include <memory>

#ifdef BR_PLATFORM_WINDOWS
	
#ifdef Brbanje_BUILD_DINAMIC
	#ifdef Brbanje_BUILD_DLL
		#define Brbanje_API _declspec(dllexport)
		
	#else
		#define Brbanje_API _declspec(dllimport)
#endif
#else
#define Brbanje_API
	#endif
#else
	#error Brbanje supports only Windows
#endif

#define BIT(x) (1 << x)

#ifdef BR_DEBUG
	#define BR_ASSERT_ENABLED
#endif

#ifdef BR_ASSERT_ENABLED
	#define BR_CORE_ASSERT(x,...) if (!x){BR_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();} 
	#define BR_ASSERT(x,...) if (!x){BR_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();} 
#else
	#define BR_CORE_ASSERT(x,...)
	#define BR_ASSERT(x,...)
#endif

//for binding event functions
#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Brbanje
{
	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T>
	using Scope = std::unique_ptr<T>;





}