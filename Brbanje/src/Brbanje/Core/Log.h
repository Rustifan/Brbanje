#pragma once


#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


namespace Brbanje
{
	class Brbanje_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};
}

//Core Macros
#define BR_CORE_INFO(...) ::Brbanje::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BR_CORE_TRACE(...) ::Brbanje::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BR_CORE_WARN(...) ::Brbanje::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BR_CORE_ERROR(...) ::Brbanje::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BR_CORE_FATAL(...) ::Brbanje::Log::GetCoreLogger()->fatal(__VA_ARGS__) //neradi

//Client Macros
#define BR_INFO(...) ::Brbanje::Log::GetClientLogger()->info(__VA_ARGS__)
#define BR_TRACE(...) ::Brbanje::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BR_WARN(...) ::Brbanje::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BR_ERROR(...) ::Brbanje::Log::GetClientLogger()->error(__VA_ARGS__)
#define BR_FATAL(...) ::Brbanje::Log::GetClientLogger()->fatal(__VA_ARGS__) //neradi

